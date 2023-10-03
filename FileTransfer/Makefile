all: clean buildserver buildclient

buildserver:
	gcc ./client.c -o ./client.out -lm

buildclient:
	gcc ./server.c -o ./server.out -lm
clean:
	rm -rf ./client.sh ./server/server.out
