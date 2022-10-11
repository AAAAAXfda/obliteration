pub struct Section {
    name_offset: u32,
    offset: u64,
    size: u64,
    name: Vec<u8>,
}

impl Section {
    pub(super) fn new(name_offset: u32, offset: u64, size: u64) -> Self {
        Self {
            name_offset,
            offset,
            size,
            name: Vec::new(),
        }
    }

    pub fn name_offset(&self) -> u32 {
        self.name_offset
    }

    pub fn offset(&self) -> u64 {
        self.offset
    }

    pub fn size(&self) -> u64 {
        self.size
    }

    pub fn name(&self) -> &[u8] {
        self.name.as_slice()
    }

    pub fn set_name<N: Into<Vec<u8>>>(&mut self, name: N) {
        self.name = name.into();
    }
}