
# Solideye


Solideye is a free open source tool designed to be used with the game Metal Gear Solid 4. It allows the user to extract Slot, Stage, Dar and Qar archive files packaged with the game. It also allow the user to create a Dar or Qar file from a folder of files. As well as this is can also decrypt and encrypt files found in the stage folders of the MGS4 Database disc, Metal Gear Online 2 and the MGS4 Demo Package. The key option must be used for these files and the key is generally the path that the file resides in, for example, 'stage/database' or 'stage/r_sna01'

This project uses [Zlib](https://github.com/madler/zlib) by [Mark Adler](https://github.com/madler), a c++ MD5 Implmentation by Frank Thilo and Blowfish Implementation by Bruce Schneier

#### Updates
- Added ability to pack Dar and Qar files
- Added Decryption and Encryption for stage files used in the MGS4 Database, Metal Gear Online 2 and MGS4 Demo  

### To Do
 - Add delayload extraction
 - Add delayload repacking
 - Add multithreaded extract
 - Add error handling
 - Create GUI variant
 - Clean up the code

##  Usage

Currently only a CLI version of the application exists. There are various options for interacting with files. If you only require to extract a file it is possible to just drag the file you wish to extract on the executable. A dictionary file is also included in order to resolve some hashes, the result is incomplete. In order to use it you must use the -dict option. Examples are given below.

Running the program without any arguments will display usage information.

```
Solideye.exe -e path\to\stage00.dat
```
The above [-e] or [--extract] instruction will extract all files from stage00.dat to the current directory.

```
Solideye.exe -e path\to\stage00.dat -f stage
```
A [-f] or [--filetype] option can be added to speicfy the file type. The program will try to automatically resolve the file type but cannot do so in all circumstances.

```
Solideye.exe -e path\to\stage00.dat -f stage -dict dictionary.txt
```
A [-dict] or [--dictionary] option can be added to use a directory to resolve real filenames instead of hashes. after the option you must put the path to the dictionary text file

```
Solideye.exe -e path\to\stage00.dat -o another\folder\
```
A [-o] or [--output] option can be added to specify an output directory for the output to be placed in. if it is not included then it will extract to the directory that the program is in.

```
Solideye.exe "path\to\cache.qar" 
```
The same can be used for Qar, Dar or Slot files. If no option command is given it will default to extraction. This makes it possible to also drag the file on the executable if only extraction is needed.

```
Solideye.exe "path\to\cache.qar" 
```
The same can be used for Qar, Dar or Slot files. If no option command is given it will default to extraction. This makes it possible to also drag the file on the executable if only extraction is needed.

```
Solideye.exe -p "folder" -f dar
```
It is possible to pack a dar or qar file with the [-p] or [--pack] command. Where folder is the folder containing the files you wish to pack and [-f] once again being for the file type of the archive, either dar or qar. If the folder has dar or qar in the name the tool can use this to decide what the file type is omitting the need to specify it. But it is recommended to do so using the option.

```
Solideye.exe -dec cache.dar -k stage/database
```
the [-dec] or [--decrypt] option will allow you to decrypt an encrypted file in a stage folder.  the [-k] or [--key] is mandatory in this instance as the key is required to do the decryption. The key will be the path to the file starting from the stage folder.

```
Solideye.exe -enc cache.dar -k stage/database
```
Finally the [-enc] or [--encrypt] option will allow you to encrypt a file with a key of your choice. once again the [-k] or [--key] is mandatory.

## License
[MIT](LICENSE.md)
This project falls under the MIT license.
