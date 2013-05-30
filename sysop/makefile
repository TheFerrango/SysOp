all: compile rules_rsys
		@echo "In teoria ho finito!"

compile:
		@echo "Entering directory src..."
		@echo "Compiling..."; 
		cd src; gcc logs.c queue.c mainc.c -pthread;

rules_rsys:
		@echo "Generating rules file for Rsyslog.."
		touch /etc/rsyslog.d/pimpedRules.conf
		@echo "" > /etc/rsyslog.d/pimpedRules.conf
		@echo "Adding rules..."
		@echo "if \$$programname == 'tr' then /var/log/threads/tr.log" >> /etc/rsyslog.d/pimpedRules.conf
		@echo "if \$$programname == 'te' then /var/log/threads/te.log" >> /etc/rsyslog.d/pimpedRules.conf
		@echo "if \$$programname == 'td' then /var/log/threads/td.log" >> /etc/rsyslog.d/pimpedRules.conf
		@echo "if \$$programname == 'tw' then /var/log/threads/tw.log" >> /etc/rsyslog.d/pimpedRules.conf
		@echo "Adesso restarto il sistema di log"
		service rsyslog restart
