# Luke Humphreys 2012
# some definitions:
#   Zero extend: (what it says on the tin) padd out a value with 0s in the
#                significan't bits. In this case to convert a 32bit value into
#                a 64 bit value
#   Sign extned: Instead of zero extending, pad with either 1s or 0s based on
#                the sign bit of the integer


# Definitions from elf.h file in the linux headers
#       /* x86-64 relocation types */
#       #define R_X86_64_NONE		0	/* No reloc */
#       #define R_X86_64_64		1	/* Direct 64 bit  */
#       #define R_X86_64_PC32		2	/* PC relative 32 bit signed */
#       #define R_X86_64_GOT32		3	/* 32 bit GOT entry */
#       #define R_X86_64_PLT32		4	/* 32 bit PLT address */
#       #define R_X86_64_COPY		5	/* Copy symbol at runtime */
#       #define R_X86_64_GLOB_DAT	6	/* Create GOT entry */
#       #define R_X86_64_JUMP_SLOT	7	/* Create PLT entry */
#       #define R_X86_64_RELATIVE	8	/* Adjust by program base */
#       #define R_X86_64_GOTPCREL	9	/* 32 bit signed pc relative
#       					   offset to GOT */
#       #define R_X86_64_32		10	/* Direct 32 bit zero extended */
#       #define R_X86_64_32S		11	/* Direct 32 bit sign extended */
#       #define R_X86_64_16		12	/* Direct 16 bit zero extended */
#       #define R_X86_64_PC16		13	/* 16 bit sign extended pc relative */
#       #define R_X86_64_8		14	/* Direct 8 bit sign extended  */
#       #define R_X86_64_PC8		15	/* 8 bit sign extended pc relative */
#       
#       #define R_X86_64_NUM		16


/^Relocation section ..rela./ {
     
     segment=$3
     addFlag="+"
} 
/^0/ {
    # Extract raw data
    offset=$1
    x86Type=$3
    ref=$5
    sign=$6
    addendum=$7
    
    # Extrac size from type
    match(x86Type,/R_X86_64_([0-9A-Z_]*)$/,parts);
    
    switch (parts[1]) {
    case "64": 
         #define R_X86_64_64	/* Direct 64 bit  */
         type="A"
         size=8
         extend="."
         break
    case "PC32":
         #define R_X86_64_PC32  /* PC relative 32 bit signed */
         # (this is an offset from the instruction pointer
         type="R"
         size=4
         extend="."
         break
    case "32":
         #define R_X86_64_32  /* Direct 32 bit zero extended */
	 # Loaded into a register and zero extended into a virtual address
         type="A"
         size=4
         extend="Z"
         break
    case "32S":
         #define R_X86_64_32S /* Direct 32 bit sign extended */
	 # Loaded into a register and sign extended into a virtual address
         type="R"
         size=4
         extend="S"
         break
    case "16":
         #define R_X86_64_16  /* Direct 16 bit zero extended */
         type="A"
         size="2"
         extend="Z"
         break
    case "PC16":
         #define R_X86_64_PC16/* 16 bit sign extended pc relative */
         type="R"
         extend="S"
         size="2"
         break
    case "8":
         #define R_X86_64_8    /* Direct 8 bit sign extended  */
         type="A"
         extend="S"
         size="1"
         break
    case "PC8":
         #define R_X86_64_PC8    /* 8 bit sign extended pc relative */
         type="R"
         extend="S"
         size="1"
         break

        
   
    case "RELATIVE":
    case "GOT32":
    case "PLT32":
    case "COPY":
    case "GLOB_DAT":
    case "JUMP_SLOT":
    case "GOTPCREL":
        NOT YET IMPLEMENTED
#       #define R_X86_64_GOT32		3	/* 32 bit GOT entry */
#       #define R_X86_64_PLT32		4	/* 32 bit PLT address */
#       #define R_X86_64_COPY		5	/* Copy symbol at runtime */
#       #define R_X86_64_GLOB_DAT	6	/* Create GOT entry */
#       #define R_X86_64_JUMP_SLOT	7	/* Create PLT entry */
#       #define R_X86_64_GOTPCREL	9	/* 32 bit signed pc relative
#       #define R_X86_64_RELATIVE	8	/* Adjust by program base */
#       					   offset to GOT */
    }
#       
#       #define R_X86_64_NUM		16
    
    flags=sprintf("%s%s%s",type,size,addFlag)
    print flags
    printf ("%s %s %s\n", $1 , segment , $5 )
}
