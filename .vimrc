" Configure this appropriately
let g:linker_dir = "/home/lhumphreys/Linker/elf-2-link/"

" Configure the valgrind plugin
let g:valgrind_command = 'make'
let g:valgrind_arguments = 'memtest'
let g:valgrind_use_horizontal_window = 1


" Add all the tags files in export/tags to the tags path
let tagsDir=g:linker_dir . "exports/tags/"
for f in split(system("ls " . tagsDir))
        let cmd="set tags+=" . tagsDir . f 
        exec cmd
endfor

" Finally source the breakpoints plugin
let g:breakpoints_file= g:linker_dir . ".gdb_breakpoints"
let cmd = "so " .  g:linker_dir . "breakpoints.vim"
exec cmd
