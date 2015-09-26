# Dynamic DNS client / server
This is dynamic DNS server and client software. In this version i've made decision to put user accounts into MySQL database instead of unix system accounts. This approach opened a new opportunities. With database we can incorporate some web based user accounts management. Provide IP change history option for users and much more.

> _**I Assume that you have at least one domain ready to be used for dynamic purposes.**_

With dDNS-ng everyone can be dynamic DNS service provider.
## Table of contents
* [Server Requirements](#server-requirements)
* [Web environment](#web-environment)
* [Synopsis](#synopsis)
* [Installation](#installation)
  * [Basic setup](#basic-setup)
  * [Automatic installation](#automatic-installation)
  * [Manual installation](#manual-installation)
  * [Installation and configuration](#installation-and-configuration)
* [Configuration file](#configuration-file)
* [Running a server](#running-a-server)
* [Enable cronjob](#cron-job)

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
Be careful. On some systems **serial** begins with capital "S" which is not supported.<br>
**White space after semicolon is important!**<br>
Client accounts are based on MySQL database contents. Every registered user can add as many domains as he want.
## Installation
There are two ways of getting dDNS-ng system to work. Automatic and Manual. But first things first. I'll start with describing basic setup that has
to be done before we can move forward.
### Basic setup
Let's assume that you have basic system installed and with no addtional software except SSH. I'll recomend to start with [Bind](https://www.isc.org/downloads/bind/) installation.
After that you can configure your brand new domain on your own DNS server. Table below describes recomended paths to use.

| OS      | Zone Directory                | Configuration file               |
|---------|:------------------------------|:---------------------------------|
| Ubuntu  | /etc/bind/                    | /etc/bind/named.conf.local       |
| Centos  | /var/named/                   | /etc/named.rfc1912.zones         |
| FreeBSD | /usr/local/etc/namedb/master/ | /usr/local/etc/namedb/named.conf |

Despite OS dependent naming convention, you have to create zone files with names identical to domains which they describes. If you have domain example.com, your zone file
should have name: example.com.

After completing DNS part you can install Apache, MySQL and PHP (LAMP) environment. Table below will be usefull with this task.

| OS      | Command                                                            |
|---------|:-------------------------------------------------------------------|
| Ubuntu  | apt-get install apache2 php5 php5-mysql mysql-server mysql-client  |
| FreeBSD | pkg install apache22 php5 php5-mysql mysql56-server mysql56-client |
| Centos  | yum -y install httpd php php-mysql mariadb-server mariadb          |

Okay. Now we are in position to do some database tasks. We need to create one for dynamic DNS system and a user account associated whith it.
```sql
mysql> CREATE DATABASE dyndns;
mysql> GRANT ALL ON dyndns.* to 'dyndns_user'@'%' IDENTIFIED by 'someRandomPassword';
mysql> FLUSH PRIVILEGES;
```
**Note:** _% sign means any host. You can restrict here access to your database if you want._

### Automatic installation
If your server is running Ubuntu or Centos i higly recommend using install.sh script. Installation script ensures that there are all necessary
packages installed into the system before building and installing all components. The only thing to do is pasting Virtualhost declaration to apache
configuration and restart apache processes.

Go to dDNS-ng/src directory and run a command:
```bash
# ./install.sh server
```

### Manual installation
TODO

## Configuration file
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
## Running a server
Tu run installed and configured dDNS server switch to root account and run:
```bash
# ddns-server -c /etc/dyndns-server.conf
```
Program will run in background. If you want to keep dDNS server running after system reboot add it to **/etc/rc.local** file with full path to the executable.
```bash
/usr/bin/ddns-server -c /etc/dyndns-server.conf
```
## Cron job
The last important thing to do is run a cron.php. I recomend using short - one minute interval
```
* * * * * /usr/local/bin/curl -s http://website.example.com/cron.php
```
Cron.php is responsible for managing mailing queue.
