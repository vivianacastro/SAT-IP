#!/bin/bash

cd Reductor

sudo apt-get update
sudo apt-get install openjdk-8-jdk
sudo apt-get install maven
mvn clean install
mvn spring-boot:run
