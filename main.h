/**
 * Header file for main.cpp
 *
 * @author Vladan Kudlac, xkudla15
 * @date created on 25.04.2021
 */

#ifndef KKO_MAIN_H_
#define KKO_MAIN_H_

#include <iostream>
#include <fstream> // ifstream
#include <unistd.h> // getopt
#include <map> // std::map
#include <vector> // std::vector

#define N 256
#define NODES 2*N + 1

#define EXIT_ARG 1 // error when parsing arguments
#define EXIT_IO  2 // error when reading/writing file

#include "Node.h"
#include "HuffmanTree.h"

#endif
