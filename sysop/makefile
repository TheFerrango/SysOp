all: compile rules_rsys
		@echo "In teoria ho finito!"

compile:
		@echo "Entering directory src..."
		@echo "Compiling..."; 
		cd src; gcc logs.c queue.c mainc.c -pthread -o prog2; chmod 711 prog2;

rules_rsys:
		@echo "Generating rules file for Rsyslog"
		touch /etc/rsyslog.d/progTwoRules.conf
		@echo "" > /etc/rsyslog.d/progTwoRules.conf
		@echo "Adding rules"
		@echo "if \$$programname == 'tr' then /var/log/threads/tr.log" >> /etc/rsyslog.d/progTwoRules.conf
		@echo "if \$$programname == 'te' then /var/log/threads/te.log" >> /etc/rsyslog.d/progTwoRules.conf
		@echo "if \$$programname == 'td' then /var/log/threads/td.log" >> /etc/rsyslog.d/progTwoRules.conf
		@echo "if \$$programname == 'tw' then /var/log/threads/tw.log" >> /etc/rsyslog.d/progTwoRules.conf
		@echo "Adesso restarto il sistema di log"
		service rsyslog restart

install: 
		@echo "Entering directory src"
		@echo "Moving files"
		cp src/prog2 /usr/bin/
		@echo "Setting permissions"
		chmod 711 /usr/bin/prog2
		@echo "Application installed"

clean:
		@echo "Cleaning source directory"
		rm src/prog2
		@echo "Done"