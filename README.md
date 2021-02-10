
# Solideye


Solideye is a free open source tool designed to be used with the game Metal Gear Solid 4. It allows the user to extract Slot, Stage, Dar and Qar archive files packaged with the game

This project uses [Zlib](https://github.com/madler/zlib) by [Mark Adler](https://github.com/madler)

### To Do
 - Add multithreaded extract
 - Add error handling
 - Create GUI variant
 - Clean up the code

##  Usage

Currently only a CLI version of the application exists. The program is primitive and just takes the filename as the only required argument. An optional output directory can also be added. It is also possible to just drag the file you wish to extract on the executable. A dictionary file is also included in order to resolve some hashes, the result is incomplete.

If you wish to use the dictionary to resolve some filenames place it alonside Solideye.exe, please note this program will only work with MGS4.

```
Solideye.exe "path\to\stage00.dat"
```
The above instruction will extract all files from stage00.dat to the current directory.

```
Solideye.exe "path\to\cache.qar" 
```
The same can be used for Qar, Dar or Slot files.

```
Solideye.exe "path\to\slot_weapon.slot" "path\to\output"
```
An optional output path can be added at the end, if it is not included then it will extract to the directory of the file being extracted.

## License
[MIT](LICENSE.md)
This project falls under the MIT license.