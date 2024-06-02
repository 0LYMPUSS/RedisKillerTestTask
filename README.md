# REDIS KILLER

This project is a test task. Basically, it's a simple key-value storage which can be accessed via tcp protocol. The values are stored in-memory, data can be accessed via folowwing commands:
```
PUT <key> <value>
GET <key>
DEL <key>
COUNT
DUMP <file-name>
```

# BUILD INSTRUCTIONS
## Supported platforms
This code has been tested on:
```
UNIX (UBUNTU)
WINDOWS
```
## Requirements
Some utilities that are needd to build the project. Skip this step if you have these utilities already installed.sudo
### git
```
sudo apt-get install git-all
```
### cmake
Minimum required version is 3.11
```
sudo apt-get install cmake
```
### Compiler
```
sudo apt-get install g++
```
### PkgConfig
```
sudo apt-get install pkg-config
```


## Instructions
Clone the repository into your working folder
```
git clone https://github.com/0LYMPUSS/RedisKillerTestTask.git
```
Go into the repository directory
```
cd RedisKillerTestTask/
```
And run cmake to configure the project.
```
cmake ./
```
Use the following command to build executables:
```
cmake --build ./
```



# USAGE
## Launching
### server
Use the following command to start the server:
```
./server -p 8888 -n 2
```
where 8888 is the server port, 2 is the max number of clients that can connect at a time. 

Use the following command if you have a startup file to initialize storage values:
```
./server -n 2 -p 8888 -f my_dump.txt
```
where my_dump.txt is a valid dump-file name.
The order in which the arguments are provided is not important.

### client
Use the following command to start the client:
```
./client localhost 8888
```
where the first argument is the name of the host and the second argument is the port number.

## Commands
These are commands that can be sent by client.
### PUT \<key\> \<value\>
Used to put values into storage.

Usage example with responses:
```
PUT name Jake
OK
PUT flower daffodil
OK
PUT flower rose
OK daffodil
```
When putting a value with an already existent key the previous value is discarded and returned with the OK message.


### GET \<key\>
Retrieves a value from storage by given key.

Usage example with responses:
```
GET name
NE
PUT name Jake
OK
GET name
OK Jake
```


### DEL \<key\>
Deletes a key-value pair from storage based on provided key.

Usage example with responses:
```
DEL name
NE
PUT name Jake
OK
DEL name
OK Jake
```


### COUNT
Returns current amount of key-value pairs in the storage.

Usage example with responses:
```
COUNT
OK 0
PUT name Jake
OK
COUNT
OK 1
DEL name
OK
COUNT
OK 0
```

### DUMP \<file-name\>
Saves the contents of the storage to provided file that can later be used on server startup to initialize the storage.

Usage example:
```
DUMP my_dump.txt
```




















