#include "Emulator/Log.h"

#include "Kyty/Core/DbgAssert.h"
#include "Kyty/Core/File.h"
#include "Kyty/Core/String.h"
#include "Kyty/Core/Subsystems.h"
#include "Kyty/Core/Threads.h"
#include "Kyty/Core/Vector.h"

#include "Emulator/Common.h"
#include "Emulator/Config.h"

#if KYTY_PLATFORM == KYTY_PLATFORM_WINDOWS
#include <windows.h> // IWYU pragma: keep
// IWYU pragma: no_include <handleapi.h>
// IWYU pragma: no_include <minwindef.h>
// IWYU pragma: no_include <processenv.h>
// IWYU pragma: no_include <winbase.h>
// IWYU pragma: no_include <wincon.h>
#endif

#ifdef KYTY_EMU_ENABLED

namespace Kyty {

namespace Log {

static bool                     g_log_initialized    = false; // true
static Core::Mutex*             g_mutex              = nullptr; // new Core::Mutex;
static Direction                g_dir                = Direction::Console; // Config::GetPrintfDirection()
static Core::File*              g_file               = nullptr;
static bool                     g_colored_printf     = false;
static thread_local Core::File* g_thread_local_file  = nullptr;
static Vector<Core::File*>*     g_thread_local_files = nullptr; // new Vector<Core::File*>;

bool IsColoredPrintf()
{
	return g_colored_printf;
}

String RemoveColors(const String& str)
{
	uint32_t start = 0;
	String   ret;
	for (;;)
	{
		auto index = str.FindIndex(U'\x1b', start);
		if (!str.IndexValid(index))
		{
			ret += str.Mid(start);
			break;
		}
		ret += str.Mid(start, index - start);
		index = str.FindIndex(U'm', index);
		if (!str.IndexValid(index))
		{
			break;
		}
		start = index + 1;
	}
	return ret;
}

Direction GetDirection()
{
	EXIT_IF(!Log::g_log_initialized);

	return g_dir;
}

void SetOutputThreadLocalFile(const String& file_name, Core::File::Encoding enc)
{
	EXIT_IF(!Log::g_log_initialized);
	EXIT_IF(Log::g_dir != Log::Direction::Directory);
	EXIT_IF(Log::g_thread_local_file != nullptr);
	EXIT_IF(g_thread_local_files == nullptr);

	Core::File::CreateDirectories(file_name.DirectoryWithoutFilename());

	g_thread_local_file = new Core::File;
	g_thread_local_file->Create(file_name);

	if (g_thread_local_file->IsInvalid())
	{
		::printf("Can't create log file: %s\n", file_name.C_Str());
		delete g_thread_local_file;
		g_thread_local_file = nullptr;
	} else
	{
		g_thread_local_file->SetEncoding(enc);
		g_thread_local_file->WriteBOM();
	}

	g_mutex->Lock();
	g_thread_local_files->Add(g_thread_local_file);
	g_mutex->Unlock();
}

void CreateThreadLocalFile()
{
	auto file_name = String::FromPrintf("%s/%d.txt", Config::GetPrintfOutputFolder().C_Str(), Core::Thread::GetThreadIdUnique());
	SetOutputThreadLocalFile(file_name, Core::File::Encoding::Utf8);
}

} // namespace Log

void emu_printf(const char* format, ...)
{
	EXIT_IF(!Log::g_log_initialized);

	EXIT_IF(Log::g_mutex == nullptr);

	Log::g_mutex->Lock();
	{
		va_list args {};
		va_start(args, format);
		String s;
		s.Printf(format, args);
		va_end(args);

		if (!Log::g_colored_printf)
		{
			s = Log::RemoveColors(s);
		}

		::printf("%s", s.C_Str());

		if (Log::g_dir == Log::Direction::File && Log::g_file != nullptr)
		{
			Log::g_file->Write(s);
		}
	}
	Log::g_mutex->Unlock();
}

void printf(const char* format, ...)
{
	EXIT_IF(!Log::g_log_initialized);

	if (Log::g_dir == Log::Direction::Silent)
	{
		return;
	}

	EXIT_IF(Log::g_mutex == nullptr);

	va_list args {};
	va_start(args, format);
	String s;
	s.Printf(format, args);
	va_end(args);

	if (!Log::g_colored_printf)
	{
		s = Log::RemoveColors(s);
	}

	if (Log::g_dir == Log::Direction::Console)
	{
		Log::g_mutex->Lock();
		::printf("%s", s.C_Str());
		Log::g_mutex->Unlock();
	} else if (Log::g_dir == Log::Direction::File && Log::g_file != nullptr)
	{
		Log::g_mutex->Lock();
		Log::g_file->Write(s);
		Log::g_mutex->Unlock();
	} else if (Log::g_dir == Log::Direction::Directory)
	{
		if (Log::g_thread_local_file == nullptr)
		{
			Log::CreateThreadLocalFile();
		}
		if (Log::g_thread_local_file != nullptr)
		{
			Log::g_thread_local_file->Write(s);
		}
	}
}

} // namespace Kyty

#endif // KYTY_EMU_ENABLED
