#include "proc.h"
#include <elf.h>
#include "ramdisk.h"
#include "fs.h"

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

// static uintptr_t loader(PCB *pcb, const char *filename) {
//   // TODO();

//   uintptr_t program_end = 0;

//   Elf_Ehdr ehdr;
//   ramdisk_read(&ehdr, 0, sizeof(Elf_Ehdr));
//   // int fd = fs_open(filename, 0, 0);
//   // fs_read(fd, &ehdr, sizeof(Elf_Ehdr));

//   // check magic number
//   assert(*(uint32_t *)ehdr.e_ident == 0x464c457f);
//   assert(ehdr.e_ident[EI_CLASS] == ELFCLASS32);
  
//   Elf_Phdr phdr;
//   uint32_t ph_offset = ehdr.e_phoff;
//   for (int i = 0; i < ehdr.e_phnum; i++) {
//     ramdisk_read(&phdr, ph_offset + i * sizeof(Elf_Phdr), sizeof(Elf_Phdr));
//     // fs_lseek(fd, ph_offset + i * sizeof(Elf_Phdr), SEEK_SET);
//     // fs_read(fd, &phdr, sizeof(Elf_Phdr));
//     if (phdr.p_type == PT_LOAD) {
//       // load segment
//       // fs_lseek(fd, phdr.p_offset, SEEK_SET);
//       // fs_read(fd, (void *)phdr.p_vaddr, phdr.p_filesz);
//       ramdisk_read((void *)phdr.p_vaddr, phdr.p_offset, phdr.p_filesz);
//       // zero the rest bytes
//       memset((void *)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
      
//       uintptr_t current_end = phdr.p_vaddr + phdr.p_memsz;
//       if (current_end > program_end) {
//         program_end = current_end;
//       }
//     }
//   }
//   pcb -> max_brk = program_end;

//   return ehdr.e_entry;
// }

static uintptr_t loader(PCB *pcb, const char *filename) {
  // TODO();

  uintptr_t program_end = 0;

  Elf_Ehdr ehdr;
  // ramdisk_read(&ehdr, 0, sizeof(Elf_Ehdr));
  printf("loader: filename=%s\n", filename);
  int fd = fs_open(filename, 0, 0);
  fs_read(fd, &ehdr, sizeof(Elf_Ehdr));

  // check magic number
  assert(*(uint32_t *)ehdr.e_ident == 0x464c457f);
  assert(ehdr.e_ident[EI_CLASS] == ELFCLASS32);
  
  Elf_Phdr phdr;
  uint32_t ph_offset = ehdr.e_phoff;
  for (int i = 0; i < ehdr.e_phnum; i++) {
    // ramdisk_read(&phdr, ph_offset + i * sizeof(Elf_Phdr), sizeof(Elf_Phdr));
    fs_lseek(fd, ph_offset + i * sizeof(Elf_Phdr), SEEK_SET);
    fs_read(fd, &phdr, sizeof(Elf_Phdr));
    if (phdr.p_type == PT_LOAD) {
      // load segment
      fs_lseek(fd, phdr.p_offset, SEEK_SET);
      fs_read(fd, (void *)phdr.p_vaddr, phdr.p_filesz);
      // zero the rest bytes
      memset((void *)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
      
      uintptr_t current_end = phdr.p_vaddr + phdr.p_memsz;
      if (current_end > program_end) {
        program_end = current_end;
      }
    }
  }
  pcb -> max_brk = program_end;

  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
