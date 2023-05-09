#!/bin/bash
docker build --tag exlang:latest docker/
docker run -d --name exlang --mount src="$(pwd)",target=/app,type=bind exlang:latest sleep infinity
