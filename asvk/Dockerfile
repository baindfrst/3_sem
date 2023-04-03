FROM gcc:4.9
COPY /fordockerserver /fordockerserver/main
WORKDIR /fordockerserver
RUN g++ -std=c++11 Source1.cpp ProccesorsInfo.cpp WorkerInfo.cpp ConnectionInfo.cpp Readingxml.cpp VectorsWork.cpp -o main
CMD ["./main"]