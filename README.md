# Dynamic DNS client / server
This is dynamic DNS server and client software. In this version i've made decision to put user accounts into MySQL database instead of unix system accounts. This approach opened a new opportunities. With database we can incorporate some web based user accounts management. Provide IP change history option for users and much more.

> _**I Assume that you have at least one domain ready to be used for dynamic purposes.**_

With dDNS-ng everyone can be dynamic DNS service provider.

## Server Requirements
* Operating system: Linux or FreeBSD
* DNS software: Bind
* MySQL database
* SMTP server (i.e. postfix)
* Platforms: x86, x86_64

## Web environment
 * apache webserver
 * PHP with mysql support

## Synopsis
Only authenticated clients are authorized to update particular zonefile. DNS zone with serial number entry should end with comment string:
```
; serial
```
**White space after semicolon is important!**<br>
Client accounts are based on MySQL database contents. Every registered user can add as many domains as he want.
## Installation
dDNS server code is linked to MySQL libs. To build server you need to install MySQL devel libs.
Source code can be obtained from [Download page](https://github.com/kkrolikowski/dyndns/wiki/Download). You can clone it direcly from Github as well.<br>

| OS      | Command                                   |
|---------|:------------------------------------------|
| Ubuntu  | apt-get install libmysqlclient-dev        |
| FreeBSD | pkg install mysql56-client                |
| Centos  | yum -y install mariadb-devel mariadb-libs |

#### Database preparation
Running dDNS-ng for the first time we have to create new database and database user account with decent privileges to tablespace. This can be done with these steps<br>
```sql
mysql> CREATE DATABASE dyndns;
mysql> GRANT ALL ON dyndns.* to 'dyndns_user'@'%' IDENTIFIED by 'someRandomPassword';
mysql> FLUSH PRIVILEGES;
```
### Installation and configuration
Once you allready have new database and database user associated with it you are in position to run **install.sh** script. Go to dDNS-ng/src directory and run a command:<br>
```bash
# ./install.sh server
```
This is an interactive script, so follow the instructions appearing on the screen. In result you will have server (or client) binary and associated configuration file copied in proper place. Installation script will also ask you to provide several data to configure your website. At the end **install.sh** script will display VirtualHost configuration ready to paste into apache files.


#### Configuration file
Server (and client) cofiguration file comes with few defaults which you can change if you want. All of the options are summarized below<br>

| Option    | Default | Description                                         |
|-----------|:-------:|:----------------------------------------------------|
| zones     | none    | absolute path to bind directory where zonefiles are stored. **IMPORTANT NOTE!**: path have to end with trailing / |
| named_conf | none | absolute path to named.conf or other file which can store zones definitions |
| listen_on | 12345   | TCP port on which server will wait for connections. Please make exception in firewall rules if necessary.
| log       | none    | absolute path to logfile
| pid       | /var/run/ddns-server.pid | pidfile location
| db_host   | localhost | your MySQL server address. Leave it unchanged if you run it on the same machine like ddns-server
| db_name   | database | Database name
| db_user   | user     | Database user account
| db_secret | password | Database user's password.
| smtp_ip   | 127.0.0.1 | SMTP server IP Address. Leave it unchanged if you run it on the same machine like ddns-server
| smtp_port | 25 | SMTP server port. Leave it unchanged if it runs on standard 25/tcp port
| mail_from | dyndns@example.com | email address which will be set as sender address (From). It can be alias to your existing account
#### Running a server
Tu run installed and configured dDNS server switch to root account and run:
```bash
# ddns-server -c /etc/dyndns-server.conf
```
Program will run in background. If you want to keep dDNS server running after system reboot add it to **/etc/rc.local** file with full path to the executable.
```bash
/usr/bin/ddns-server -c /etc/dyndns-server.conf
```
