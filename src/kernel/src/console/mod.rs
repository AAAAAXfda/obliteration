use crate::errno::Errno;
use crate::fs::{VFile, VFileOps, VPath};
use crate::process::VThread;
use crate::ucred::Ucred;
use macros::vpath;
use std::fmt::{Display, Formatter};

/// An implementation of `/dev/console`.
#[derive(Debug)]
pub struct Console {}

impl Console {
    pub const PATH: &VPath = vpath!("/dev/console");

    pub fn new() -> Self {
        Self {}
    }
}

impl VFileOps for Console {
    fn ioctl(
        &self,
        _file: &VFile,
        _com: u64,
        _data: &[u8],
        _cred: &Ucred,
        _td: &VThread,
    ) -> Result<(), Box<dyn Errno>> {
        // TODO: Implement this.
        Ok(())
    }
}

impl Display for Console {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        Self::PATH.fmt(f)
    }
}
