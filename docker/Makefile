.PHONY: all mosquitto postgres pgadmin run clean

all: mosquitto postgres pgadmin

mosquitto:
	mkdir -p mosquitto/data
	mkdir -p mosquitto/log
	mkdir -p mosquitto/config
	-cp ./mosquitto.conf mosquitto/config

postgres:
	mkdir -p postgres/data
	mkdir -p postgres/init

pgadmin:
	mkdir -p pgadmin
#	chmod ugo+w pgadmin

run:all
	sudo docker-compose up #-d

clean:
	-sudo $(RM) -fr mosquitto
	-sudo $(RM) -fr postgres
	-sudo $(RM) -fr pgadmin
