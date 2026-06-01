libusb_dep = dependency('libusb-1.0')
threads_dep = dependency('threads')

add_project_arguments('-D_FILE_OFFSET_BITS=64', language : 'c')

src_core = files(
  'flashcmd_api.c',
  'spi_controller.c',
  'spi_nand_flash.c',
  'spi_nor_flash.c',
  'ch341a_spi.c',
  'timer.c',
  'main.c',
)

src_eeprom = files(
  'ch341a_i2c.c',
  'i2c_eeprom.c',
  'spi_eeprom.c',
  'bitbang_microwire.c',
  'mw_eeprom.c',
  'ch341a_gpio.c',
)

snander_src = src_core
snander_args = []

if get_option('eeprom_support')
  snander_src += src_eeprom
  snander_args += '-DEEPROM_SUPPORT'
endif

win_res = []
if host_machine.system() == 'windows'
  win = import('windows')
  win_res = win.compile_resources('snander.rc')
endif

executable('SNANDer',
  snander_src + win_res,
  c_args : snander_args,
  dependencies : [libusb_dep, threads_dep],
  install : true,
)
