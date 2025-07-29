# Enable loading of local .gdbinit (for GDB >= 10)
set auto-load local-gdbinit on

# Set default radix to hex
set output-radix 16

# Define a typed pointer to the peripheral base address
set $PORT = (volatile uint32_t *) 0x41004100

# Define a macro to print PORT_OUT register
define port_out
  p *(volatile uint32_t*)0x41004110
end

# Define a macro to print PORT_IN register
define portb_in
  p *(volatile uint32_t*)0x410044A0
end
define portb_in_b
  p/t *(volatile uint32_t*)0x410044A0
end

#High Score Defines
define simon
  echo simon Info\n
  x/12xb 0x0003fc18
end

define simon-e
  echo [Easy] High Score:\n
  p/d *(volatile uint32_t*)0x0003fc18
end

define simon-m
  echo [Medium] High Score:\n
  p/d *(volatile uint32_t*)0x0003fc1c
end

define simon-h
  echo [Hard] High Score:\n
  p/d *(volatile uint32_t*)0x0003fc20
end

define lightdash
  echo Light Dash Info\n
  x/12xb 0x0003fc30
end

define lightdash-e
  echo [Easy] High Score:\n
  p/d *(volatile uint32_t*)0x0003fc30
end

define lightdash-m
  echo [Medium] High Score:\n
  p/d *(volatile uint32_t*)0x0003fc34
end

define lightdash-h
  echo [Hard] High Score:\n
  p/d *(volatile uint32_t*)0x0003fc38
end

# Define a macro to toggle a pin (example: pin 17)
define toggle_pin17
  set *(volatile uint32_t*)0x41004110 ^= (1 << 17)
end

# Define a macro to print multiple words from a memory block
define dump_words
  x/10wx 0x20000000
end

# Comment: Add more macros as needed for peripherals
