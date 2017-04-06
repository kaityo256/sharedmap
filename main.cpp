//------------------------------------------------------------------------
#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <sstream>
#include <random>
#include "serializablemap.hpp"
//------------------------------------------------------------------------
int rank, procs;
//------------------------------------------------------------------------
template<class T>
void
SendrecvVector(int dest_rank, std::vector<T> &send_buffer, std::vector<T> &recv_buffer) {
  int recv_size = 0;
  int send_size = send_buffer.size();
  MPI_Status st;
  MPI_Sendrecv(&send_size, 1, MPI_INT, dest_rank, 0, &recv_size, 1, MPI_INT, dest_rank, 0, MPI_COMM_WORLD, &st);
  recv_buffer.resize(recv_size);
  MPI_Sendrecv(send_buffer.data(), send_size * sizeof(T), MPI_BYTE, dest_rank, 0, recv_buffer.data(), recv_size * sizeof(T), MPI_BYTE, dest_rank, 0, MPI_COMM_WORLD, &st);
}
//------------------------------------------------------------------------
template<class T>
void showall(T &m) {
  for (int i = 0; i < procs; i++) {
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == i) {
      printf("rank = %d\n", rank);
      m.show();
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
}
//------------------------------------------------------------------------
int
main(int argc, char**argv) {
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  serializable_map<std::string, int> m;
  std::mt19937 mt(rank + 1);
  std::uniform_int_distribution<int> ud(0, 10);
  for (int i = 0; i < 3; i++) {
    std::stringstream ss;
    int value = ud(mt);
    ss << "key" << std::setw(2) << std::setfill('0') << value;
    m[ss.str()] = value;
  }
  if (rank == 0) {
		std::cout << "before" << std::endl;
  }
  showall(m);
  for (int s = 0; (1 << s) < procs; s++) {
    int dest = (rank % (1 << (s + 1)) < (1 << s)) ? rank + (1 << s) : rank - (1 << s);
    std::vector<char> mybuffer = m.serialize();
    std::vector<char> recvbuffer;
    SendrecvVector(dest, mybuffer, recvbuffer);
    m.deserialize(recvbuffer);
    MPI_Barrier(MPI_COMM_WORLD);
  }
  if (rank == 0) {
		std::cout << "after" << std::endl;
  }
  showall(m);
  MPI_Finalize();
}
//------------------------------------------------------------------------
