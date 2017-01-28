#include <iostream>
#include <fstream>
#include <iomanip>

const int BAD_FILE = -1;
const int BAD_USAGE = -2;

/*
Display a description of the program's usage to stdout.
*/
void usage() {
  std::cout << "Usage: hexd [file]" << std::endl;
}

// TODO! Write bad file to stderr?
/*
Display an error message for a file that can't be opened to stdout.
*/
void bad_file(char * file_name) {
  std::cout << "Error: Could not open [" << file_name << "]" << std::endl;
}

/*
Check if byte is printable ASCII.
*/
int is_printable(char byte) {
  return (32 <= byte) && (byte <= 176);
}

/*
Print out the bytes in a given array as a series of hex
digits. There's a space between every 4 hex digits. The row size is
the amount of bytes to print. The pad size is how many digits to
replace with a space.
*/
void dump_row_bytes(char * bytes, size_t row_size, size_t pad_size) {
  int pad_flag = 0;
  for (size_t i = 0; i < row_size; i++) {
    std::cout << std::setfill('0') <<  std::setw(2) << std::hex << (int)(unsigned char)bytes[i];
    if (pad_flag) {
      std::cout << " ";
    }
    pad_flag = !pad_flag;
  }
  std::cout << " ";
  for (size_t i = 0; i < pad_size; i++) {
    std::cout << "  ";
    if (pad_flag) {
      std::cout << " ";
    }
    pad_flag = !pad_flag;
  }
}

/*
Print out the bytes in a given array as a series of ascii
characters. If a character cannot be printed without messing up the
formatting, like a newline, then a '.' will be printed instead. The
row_size is the length of the array.
*/
void dump_row_ascii(char * bytes, size_t row_size) {
  for (size_t i = 0; i < row_size; i++) {
    if (is_printable(bytes[i])) {
      std::cout << bytes[i];
    } else {
      std::cout << ".";
    }
  }
}

/*
Print out a hexdump of a given byte array. The first argument, line,
is the address of the first byte. It's assumed that the row is taken
from a larger blob, so the first byte's address will not always be
zero. The row size is the amount of bytes in the array to print. The
pad size is how many more bytes of padding the hexdump needs. Pad size
should usually be zero unless the last row of a file is being dumped.
 */
void dump_row(int line, char * bytes, size_t row_size, size_t pad_size) {
  std::cout << std::setfill('0') << std::setw(7) << std::hex << line << ": ";
  dump_row_bytes(bytes, row_size, pad_size);
  dump_row_ascii(bytes, row_size);
  std::cout << std::endl;
}

/*
Hexdump a file in sixteen byte increments. Each sixteen bytes will
appear on a new line with their address relative the file, their hex
values, and then their ascii values.
*/
void dump_file(std::ifstream & file) {
  char row[16] = {0};
  int line = 0;
  
  while (!file.eof()) {
    file.read(row, 16);
    if (file.gcount() == 0) {
      return;
    }
    dump_row(line, row, file.gcount(), 16 - file.gcount());
    for (int i = 0; i < 16; i++) {
      row[i] = 0;
    }
    line += 16;
  }
}


/*
Take a file and print out a hexdump of it to stdout.
*/
int main(int argc, char ** argv) {
  if (argc != 2) {
    usage();
    return BAD_USAGE;
  }
  char * file_name = argv[1];
  std::ifstream file(file_name, std::ios::binary);
  
  if (!file) {
    bad_file(file_name);
    return EXIT_FAILURE;
  }
  dump_file(file);  
  file.close();
  
  return EXIT_SUCCESS;
}
