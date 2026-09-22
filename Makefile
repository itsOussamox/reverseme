.DEFAULT_GOAL := help

.PHONY: help level1 clean-level1 fclean-level1 level2 clean-level2 fclean-level2 level3 clean-level3 fclean-level3 clean fclean

help:
	@printf '\nReverseMe Docker environments\n'
	@printf '============================\n\n'
	@printf 'Run a level:\n'
	@printf '  make level1          Build and open a shared Level 1 Bash session\n'
	@printf '  make level2          Build and open a shared Level 2 Bash session\n'
	@printf '  make level3          Build and open a shared Level 3 Bash session\n\n'
	@printf 'Clean up:\n'
	@printf '  make clean-level1    Remove the Level 1 container\n'
	@printf '  make clean-level2    Remove the Level 2 container\n'
	@printf '  make clean-level3    Remove the Level 3 container\n'
	@printf '  make clean            Remove all level containers\n'
	@printf '  make fclean           Remove all level containers and images\n\n'

level1:
	docker build --platform linux/amd64 -f level1/Dockerfile -t reverseme-level1 .
	@if ! docker inspect reverseme-level1 >/dev/null 2>&1; then docker run -d -it --name reverseme-level1 reverseme-level1; elif [ "$$(docker inspect -f '{{.State.Running}}' reverseme-level1)" != "true" ]; then docker start reverseme-level1; fi
	docker exec -it reverseme-level1 /bin/bash

clean-level1:
	-docker rm -f reverseme-level1

fclean-level1: clean-level1
	-docker rmi reverseme-level1

level2:
	docker build --platform linux/amd64 -f level2/Dockerfile -t reverseme-level2 .
	@if ! docker inspect reverseme-level2 >/dev/null 2>&1; then docker run -d -it --name reverseme-level2 reverseme-level2; elif [ "$$(docker inspect -f '{{.State.Running}}' reverseme-level2)" != "true" ]; then docker start reverseme-level2; fi
	docker exec -it reverseme-level2 /bin/bash

clean-level2:
	-docker rm -f reverseme-level2

fclean-level2: clean-level2
	-docker rmi reverseme-level2

level3:
	docker build --platform linux/amd64 -f level3/Dockerfile -t reverseme-level3 .
	@if ! docker inspect reverseme-level3 >/dev/null 2>&1; then docker run -d -it --name reverseme-level3 reverseme-level3; elif [ "$$(docker inspect -f '{{.State.Running}}' reverseme-level3)" != "true" ]; then docker start reverseme-level3; fi
	docker exec -it reverseme-level3 /bin/bash

clean-level3:
	-docker rm -f reverseme-level3

fclean-level3: clean-level3
	-docker rmi reverseme-level3

clean: clean-level1 clean-level2 clean-level3

fclean: fclean-level1 fclean-level2 fclean-level3