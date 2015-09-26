#!/usr/bin/env bash
#
# dDNS-ng install script
# Author: Krzysztof Krolikowski <kkrolikowski@gmail.com>
#

function checkDistro() {
	if [ -f "/etc/debian_version" ]; then
		echo "deb"
	else
		echo "rpm"
	fi
}
function installpkg() {
	package=$1
	echo -n "$package: "

	if [ $package_system == "deb" ]; then
		dpkg -l | grep $package |grep -v makedev 2>&1 >/dev/null
		if [ $? -eq 0 ]; then
			echo "installed"
		else
			echo -n "Installing........ "
			apt-get -qy install $package 2>&1 >/dev/null
			echo "[done]"
		fi
		if [ $package == 'php5-mcrypt' ]; then
			php5enmod mcrypt 2>&1 >/dev/null
		fi
		if [ $package == 'php-pear' ]; then
			pear install Mail 2>&1 >/dev/null
		fi
	else
		if [ $package == 'libmysqlclient' ]; then
			package="mariadb-libs"
		fi
		if [ $package == "libmysqlclient-dev" ]; then
			package="mariadb-devel"
		fi
		if [ $package == "php5-mcrypt" ]; then
			package="php-mcrypt"
			rpm -qa |grep epel 2>&1 >/dev/null
			if [ $? -eq 1 ]; then
				yum -y -q install epel-release 2>&1 >/dev/null
			fi
		fi
		if [ $package == 'php-pear' ]; then
			package="php-pear-Mail"
		fi
		rpm -qa |grep -w $package 2>&1 >/dev/null
		if [ $? -eq 0 ]; then
			echo "installed"
		else
			echo -n "Installing........ "
			yum -y -q install $package 2>&1 >/dev/null
			echo "[done]"
		fi
	fi
}
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
package_system=$(checkDistro);
if [ $package_system == "deb" ]; then
	echo -n "Rebuilding package indexes........... "
	apt-get -qy update 2>&1 >/dev/null
	echo "[done]"
fi
echo "				>> Collecting inventory <<				"

installpkg make
installpkg gcc
installpkg libmysqlclient
installpkg libmysqlclient-dev
installpkg php5-mcrypt
installpkg php-pear

if [ $1 == "server" ] ; then
	if [ ! -f "ddns-server" ] ; then
		echo -n "Building server...... "
		$MAKE server 2>&1 >/dev/null
		echo "[done]"
	fi
	echo "Installing files"
	if [[ -f "$BINDIR/ddns-server" && -f "$CONFDIR/dyndns-server.conf" ]] ; then
		echo "Files allready installed, moving on..."
	else
		cp ddns-server $BINDIR/
		cp ../doc/dyndns-server.conf $CONFDIR/
		echo
		echo "!!!!		Please edit $CONFDIR/dyndns-server.conf		!!!!"
		echo
	fi

	default="example.com"
	read -p "Initial domain [$default]: " -a FIRSTDOMAIN
	FIRSTDOMAIN=${FIRSTDOMAIN:-$default}

	echo
	echo "				>> Preparing database <<			"
	echo
	read -p "DB host: " DBHOST
	read -p "DB user: " DBUSER
	read -s -p "DB password: " DBPASS
	echo
	read -p "DB name: " DBNAME
	which mysql 2>&1 > /dev/null
	if [ $? -eq 1 ]; then
		echo "Please install mysql client"
		exit 1
	fi
	echo
	echo "				>> Installing Database <<			"
	mysql -h$DBHOST -u$DBUSER -p$DBPASS $DBNAME < ../doc/dbschema.sql 1> /dev/null 2> $ERR
	if [ $? == 1 ] ; then
		cat $ERR
		unlink $ERR
		exit 1
	fi
	echo "				>> Allmost ready. Lets create initial admin account <<"
	read -p "Login: " login
	read -s -p "Password: " pass
	md5pass=`echo $pass |openssl passwd -1 -stdin`
	echo
	IFS= read -r -p "Name: " name
	read -p "E-mail: " email
	mysql -h$DBHOST -u$DBUSER -p$DBPASS $DBNAME -e "INSERT INTO users(login,pass,role,active,name,email) VALUES('$login','$md5pass','admin',1,'$name','$email');" 1> /dev/null 2>> $ERR
	mysql -h$DBHOST -u$DBUSER -p$DBPASS $DBNAME -e "INSERT INTO domains(domain,status,user_id,owner,ttl,serial,refresh,retry,expiry,maximum,domainstatus) VALUES('$FIRSTDOMAIN','public',0,'root',0,0,1200,1200,2419200,86400,'active');" 1> /dev/null 2>> $ERR
	mysql -h$DBHOST -u$DBUSER -p$DBPASS $DBNAME -e "INSERT INTO subdomains(user_id,domain_id,subdomain,type,dynamic) VALUES(1,1,'admin','A',1);" 1> /dev/null 2>> $ERR
	if [ $? == 1 ] ; then
		cat $ERR
		unlink $ERR
		exit 1
	fi
	echo "Database ready."
	echo
	echo "				>> Preparing web interface <<			"
	echo
	echo "DNS information"
	default="hostmaster.example.com"
	read -p "Hostmaster [$default]: " -a HOSTMASTER
	HOSTMASTER=${HOSTMASTER:-$default}

	default="ns1.example.com"
	read -p "Master DNS [$default]: " -a MASTERDNS
	MASTERDNS=${MASTERDNS:-$default}

	default="86400"
	read -p "TTL [$default]: " -a TTL
	TTL=${TTL:-$default}

	default="ns1.example.com"
	read -p "DNS Servers [$default dns2 dns3 ...]: " -a SECONDARY
	SECONDARY=${SECONDARY:-$default}
	len=${#SECONDARY[@]}
	i=0
	DNSLIST=()
	for dns in ${SECONDARY[@]}; do
		if [ $i -eq $(($len-1)) ]; then
			ns="'$dns.'"
		else
			ns="'$dns.', "
		fi
		DNSLIST+=$ns
		((i++))
	done

	default="mx.example.com"
	read -p "MX Server [$default]: " -a MX_SRV
	MX_SRV=${MX_SRV:-$default}

	default="admin@example.com"
	read -p "Service email [$default]: " -a MAIN_EMAIL
	MAIN_EMAIL=${MAIN_EMAIL:-$default}

	echo
	echo "				>> Installing admin interface <<		"
	echo
	read -p "Enter site address: " SITENAME

	if [ -d "/var/www/html" ] ; then
		ROOT="/var/www/html"
	else
		ROOT="/var/www"
	fi
	DEST="$ROOT/$SITENAME/htdocs"
	mkdir -p $DEST
	cp -r ../www/* $DEST/

	wget -q http://files.ddns-ng.net/Smarty-3.1.8.tar.gz -O /tmp/Smarty.tar.gz
	tar xf /tmp/Smarty.tar.gz -C /tmp/
	mkdir -p $DEST/lib/Smarty
	cp -r /tmp/Smarty-3.1.8/libs/* $DEST/lib/Smarty/
	rm -fr /tmp/Smarty-3.1.8

	mkdir $DEST/templates_c
	chmod 777 $DEST/templates_c

	WEBCONFIG="$DEST/inc/config.php"
	cat > $WEBCONFIG <<EOL
<?php
/*
 * Database connection
*/
        define('DBNAME', '$DBNAME');
        define('LOGIN', '$DBUSER');
        define('PASS', '$DBPASS');
        define('DB_HOST', '$DBHOST');
/*
 * DNS settings
*/
        define('HOSTMASTER', '$HOSTMASTER.');
        define('MASTERDNS', '$MASTERDNS.');
        define('TTL', '$TTL');
        define('DNS_SERVERS', serialize(array($DNSLIST)));
        define('MX_SERVER', '10 $MX_SRV.');
/*
 * Other settings
*/
        define('SMARTY_LIB', './lib/Smarty/Smarty.class.php');
        define('ADM_EMAIL', '$MAIN_EMAIL');
?>
EOL
	cat > /tmp/dyndns-apache.conf <<EOL
<VirtualHost *:80>
   ServerName $SITENAME
   DocumentRoot "$DEST"
   <Location />
      Options -Indexes
      <Limit GET POST PUT DELETE HEAD OPTIONS>
         Order allow,deny
         Allow from all
      </Limit>
   </Location>
   ErrorLog /var/log/apache/$SITENAME-error.log
   CustomLog /var/log/apache/$SITENAME-access.log combined
</VirtualHost>
EOL
	echo
	echo "				>> Installation done. Below Apache Virtualhost config <<"
	echo "				>> 		Restart apache to activate installed modules		<<"
	echo
	cat /tmp/dyndns-apache.conf
	rm /tmp/dyndns-apache.conf

elif [ $1 == "client" ] ; then
	if [ ! -f "ddns-client" ] ; then
		echo "Run: $MAKE client"
	else
		echo "Installing files"
		if [[ -f "$BINDIR/ddns-client" && -f "$CONFDIR/dyndns-client.conf" ]] ; then
			echo "Files allready installed, nothing to do..."
		else
			cp ddns-client $BINDIR/
			cp ../doc/dyndns-client.conf $CONFDIR/
			echo "Please edit $CONFDIR/dyndns-client.conf"
		fi
	fi
else
	echo "Unknown option"
fi
