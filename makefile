all:
		@echo "Compiling..."
		gcc main.c
		@echo "Generating rules file for Rsyslog.."
		touch /etc/rsyslog.d/pimpedRules.conf
		@echo "" > /etc/rsyslog.d/pimpedRules.conf
		@echo "Adding rules..."
		@echo "if \$$programname == 'tr' then /var/log/tr.log" >> /etc/rsyslog.d/pimpedRules.conf
		@echo "if \$$programname == 'te' then /var/log/te.log" >> /etc/rsyslog.d/pimpedRules.conf
		@echo "if \$$programname == 'td' then /var/log/td.log" >> /etc/rsyslog.d/pimpedRules.conf
		@echo "if \$$programname == 'tw' then /var/log/tw.log" >> /etc/rsyslog.d/pimpedRules.conf
		@echo "Adesso restarto il sistema di log"
		pkill -HUP rsyslogd
		@echo "In teoria ho finito!"
