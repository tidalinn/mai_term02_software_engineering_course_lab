docker container prune
docker image prune
docker volume prune
docker network prune
docker system prune

docker stop $(docker ps -aq)
docker rm $(docker ps -a -q)
docker rmi $(docker images -q)

# sh kill_docker.sh