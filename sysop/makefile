all: compile rules_rsys
		@echo "All done!!"

compile:
		@echo "Entering directory src..."
		@echo "Compiling..."; 
		@cd src; gcc logs.c queue.c mainc.c -pthread -o threads; chmod 711 threads;
		@echo "Compiled"

rules_rsys:
		@echo "Generating rules file for Rsyslog"
		@touch /etc/rsyslog.d/progTwoRules.conf
		@echo "" > /etc/rsyslog.d/progTwoRules.conf
		@echo "Adding rules"
		@echo "if \$$programname == 'tr' then /var/log/threads/tr.log" >> /etc/rsyslog.d/progTwoRules.conf
		@echo "if \$$programname == 'te' then /var/log/threads/te.log" >> /etc/rsyslog.d/progTwoRules.conf
		@echo "if \$$programname == 'td' then /var/log/threads/td.log" >> /etc/rsyslog.d/progTwoRules.conf
		@echo "if \$$programname == 'tw' then /var/log/threads/tw.log" >> /etc/rsyslog.d/progTwoRules.conf
		@echo "Adesso restarto il sistema di log"
		service rsyslog restart

install: compile rules_rsys
		@echo "Entering directory src"
		@echo "Moving files"
		@cp src/threads /usr/bin/
		@echo "Setting permissions"
		@chmod 711 /usr/bin/threads
		@echo "Application installed"

clean:
		@echo "Cleaning source directory"
		rm src/threads
		@echo "Done"
