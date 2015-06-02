#!/usr/bin/env bash
#
# dDNS-ng install script
#
if [ $(uname -s) == "FreeBSD" ]; then
	MAKE="gmake"
	CONFDIR="/usr/local/etc"
	BINDIR="/usr/local/bin"
else
	MAKE="make"
	CONFDIR="/etc"
	BINDIR="/usr/bin"
fi
ERR="/tmp/err"

if [ $# -lt 1 ] ; then
	echo "Usage: $0 [client|server]"
	exit 1
fi	
if [ $(id -u) -gt 0 ] ; then
	echo "Use root account"
	exit 1
fi
if [ $1 == "server" ] ; then
	if [ ! -f "ddns-server" ] ; then
		echo "Run: $MAKE server"
	else
		echo "Installing files"
		if [[ -f "$BINDIR/ddns-server" && -f "$CONFDIR/dyndns-server.conf" ]] ; then
			echo "Files allready installed, moving on..."
		else
			cp ddns-server $BINDIR/
			cp ../doc/dyndns-server.conf $CONFDIR/
			echo "Please edit $CONFDIR/dyndns-server.conf"
			echo "Press any key to proceed..."
			read
		fi
		echo "Preparing database"
		echo -n "DB host: "
		read DBHOST
		echo -n "DB user: "
		read DBUSER
		echo -n "DB password: "
		read -s DBPASS
		echo
		echo -n "DB name: "
		read DBNAME
		which mysql 2>&1 > /dev/null
		if [ $? -eq 1 ]; then
			echo "Please install mysql client"
		else
			echo "Installing Database"
			mysql -h$DBHOST -u$DBUSER -p$DBPASS $DBNAME < ../doc/dbschema.sql 1> /dev/null 2> $ERR
			if [ $? == 1 ] ; then
				cat $ERR
				unlink $ERR
				exit 1
			fi
			echo "Allmost ready. Lets create initial admin account"
			echo -n "Login: "
			read login
			echo -n "Password: "
			read -s pass
			md5pass=`echo $pass |openssl passwd -1 -stdin`
			echo
			echo -n "Name: "
			read name
			echo -n "E-mail: "
			read email
			mysql -h$DBHOST -u$DBUSER -p$DBPASS $DBNAME -e "INSERT INTO users(login,pass,role,active,name,email) VALUES('"$login"','"$md5pass"','admin',1,'"$name"','"$email"');" 1> /dev/null 2> $ERR
			if [ $? == 1 ] ; then
				cat $ERR
				unlink $ERR
				exit 1
			fi
			echo "Database ready."
		fi

	fi
fi
