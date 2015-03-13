# Dynamic DNS client and server
## Compilation
To compile this software go to src directory and type: make. If you want to compile only client or server type "make client" or "make server.
## Configuration
Sample configuration files are in doc directory. You can adjust them to your environment. There are few rules regarding editing these files.
* option and value should be separated by space, '=' and space
* directory path for zonedir (server only) must end with trailing slash '/'
* lines with hash sign (#) are ommited

## Running
After compiling software you will have binary files: ddns-server and ddns-client. You can place them and configuration files wherever you want on your system. Both client and server have only one important commandline option: -c which points to location of configuration file.
