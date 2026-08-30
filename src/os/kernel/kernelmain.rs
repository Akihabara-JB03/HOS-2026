#![no_std]
#![no_main]
use panic;
#[unsafe(no_mangle)]
pub extern "C" fn kernel_main() {
  loop {}
}
