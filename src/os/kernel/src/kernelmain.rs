#![no_std]
#![no_main]
mod panic;
#[unsafe(no_mangle)]
pub extern "C" fn kernel_main() {
  loop {}
}
