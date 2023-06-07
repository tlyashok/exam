FROM ubuntu

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update
RUN apt-get install qt6-base-dev qt6-base-dev-tools -y
RUN apt-get install build-essential git -y

WORKDIR /app
COPY . /app
RUN qmake6 
RUN make

RUN chmod +x hallServer
CMD ["./hallServer"]