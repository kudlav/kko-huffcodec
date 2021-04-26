/**
 * Main file parsing input params and controlling other components.
 *
 * @author Vladan Kudlac, xkudla15
 * @date created on 20.04.2021
 */

#include "main.h"

bool decode = false;
bool encode = false;
bool useModel = false;
bool useActiv = false;
std::string inputFile = "";
std::string outputFile = "output.raw";
int width = 0;

void printHelp() {
	std::cout << "Pouziti:  huff_codec -i <ifile> -w <width_value> -c/-d [-o <ofile>] [<parametry>]" << std::endl;
	std::cout << "Parametry: - c         režim komprese" << std::endl;
	std::cout << "           - d         režim dekomprese" << std::endl;
	std::cout << "           - m         aktivuje model" << std::endl;
	std::cout << "           - a         aktivuje aktivní skenování obrazu" << std::endl;
	std::cout << "           - i         název vstupního souboru" << std::endl;
	std::cout << "           - o         název výstupního souboru, výchozí 'output.raw'" << std::endl;
	std::cout << "           - w         šířka obrazu, hodnota >= 1" << std::endl;
	std::cout << "           - h         vypíše tuto nápovědu na výstup" << std::endl;
}

bool parseArguments(int argc, char* argv[]) {
	int c;
	while ((c = getopt(argc, argv, "cdmahi:o:w:")) != -1) {
		switch (c) {
			case 'c':
				if (decode) {
					std::cerr << "Přepínač '-c' nelze použít spolu s '-d'" << std::endl;
					return false;
				}
				encode = true;
				break;
			case 'd':
				if (encode) {
					std::cerr << "Přepínač '-d' nelze použít spolu s '-c'" << std::endl;
					return false;
				}
				decode = true;
				break;
			case 'm':
				useModel = true;
				break;
			case 'a':
				useActiv = true;
				break;
			case 'i':
				inputFile = optarg;
				break;
			case 'o':
				outputFile = optarg;
				break;
			case 'w':
				try {
					width = std::stoi(optarg);
				} catch (std::exception const&) {}
				if (width < 1) {
					std::cerr << "Parametr -w musí být nastaven na celé číslo větší než 0" << std::endl;
				}
				break;
			case 'h':
				return false;
			default:
				return false;
		}
	}

	if (inputFile == "") {
		std::cerr << "Specifikujte vstupní soubor" << std::endl;
		return false;
	} else if (!encode && !decode) {
		std::cerr << "Specifikujte buď režim dekódování nebo režim kódování" << std::endl;
		return false;
	}

	return true;
}


std::string getPath(Node* node) {
	std::string path = "";
	while (node->parent != nullptr) {
		path = (node->isLeft ? '1' : '0') + path;
		node = node->parent;
	}
	return path;
}


int main(int argc, char* argv[]) {

	// Read input
	if (!parseArguments(argc, argv)) {
		printHelp();
		return EXIT_ARG;
	}

	// Open input/output file
	std::ifstream iFile;
	iFile.open(inputFile, std::ifstream::in);
	std::ofstream oFile;
	oFile.open(outputFile, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);

	// Init Huffman Tree
	HuffmanTree tree = HuffmanTree();

	// Try to load first char
	uint8_t c = iFile.get();
	if (!iFile.good()) {
		std::cerr << "Invalid or empty file: " << inputFile << std::endl;
		return EXIT_IO;
	}

	while (iFile.good()) {
		// Encode
		Node* foundNode = tree.findNode(c);

		if (tree.isNYT(foundNode)) {
			oFile << "NYT: " << std::to_string(c) << std::endl;
		} else {
			oFile << getPath(foundNode) << std::endl;
		}

		// Update Huffman Tree
		tree.updateHuffmanTree(foundNode, c);

		// Load next symbol
		c = iFile.get();
	}

	iFile.close();
	oFile.close();

	return EXIT_SUCCESS;
}
