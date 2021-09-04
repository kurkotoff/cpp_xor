#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>
#include <string>
#include <iterator>
#include <fstream>
#include <map>
#include <string.h>
#include <boost/program_options.hpp>

/*
 * This program is designed for
 * Encrypting data with the XOR cipher.
 * Using the given key.
 */

using namespace std;
namespace opt = boost::program_options;

string my_xor(string data, string key);
string hexlify(string data);
void encrypt(string data_filename, string key_filename, string out_filename);
string read_file(string filename);

int main(int argc, char **argv){
	string data, key, output;
	
	//for (int i = 0; i < argc; i++){
	//	cout << argv[i] << " : " << i << endl;
	//}
/*	for (int i = 1; i < argc; i++){
		if (strcmp(argv[i], "-f") == 0 && data.size() == 0){
			data = argv[i+1];
			cout << "Data input file: " << argv[i+1];
		}
		else if (strcmp(argv[i], "-f") == 0 && data.size() > 0){
			cout << "error: multiple input files specified!";
			return 0;
		}

		if (strcmp(argv[i], "-k") == 0 && key.size() == 0){
			key = argv[i+1];
		}
		else if (strcmp(argv[i], "-k") == 0 && key.size() < 0){
			cout << "error: multiple key files specified!";
			return 0;
		}

		if (strcmp(argv[i], "-o") == 0 && output.size() == 0){
			output = argv[i+1];
		}
		else if (strcmp(argv[i], "-o") == 0 && output.size() > 0){
			cout << "error: multiple input files specified!";
			return 0;
		}
	} */

	opt::options_description desc("Usage");

	desc.add_options()
		("input-file,f", opt::value<string>(), "File to read data from")
		("key-file,k", opt::value<string>(), "File with key")
		("output-file,o", opt::value<string>(), "Output to <filename>")
		("help,h", "Show this message");

	opt::variables_map vm;
	opt::store(opt::parse_command_line(argc, argv, desc), vm);
	opt::notify(vm);

	if (vm.count("help")){
		cout << desc << endl;
		return 0;
	}

	if (vm.empty()){
		cout << desc << endl;
		return 0;
	}

	encrypt(vm["input-file"].as<string>(),
			vm["key-file"].as<string>(),
			vm["output-file"].as<string>());
	cout << "Encryption sucessful!";
	return 0;
}

/*
 * Main encryption function
 */

string my_xor(string data, string key){
	string cipher;
	cipher.resize(data.size());
	int j = key.size();

	for (int i = 0; i < data.size(); i++){
		char new_symbol = data[i] ^ key[i%j];
		cipher[i] = new_symbol;
	}
	
	return cipher;
}

/*
 * This function translates output into hex
 */


string hexlify(string data){
	ofstream hex_ifile;
	hex_ifile.open("hex_tmp.txt");
	
	for (const auto &symbol : data){
		if (symbol >= 16){
			hex_ifile << hex << int(symbol);
		}
		else{
			hex_ifile << "0" << hex << int(symbol);
		}
	}
	
	hex_ifile.close();
	
	ifstream hex_ofile;
	hex_ofile.open("hex_tmp.txt");
	
	string hex_str;
	getline(hex_ofile, hex_str);

	hex_ofile.close();
	remove("hex_tmp.txt");

	return hex_str;
}

void encrypt(string data_filename, string key_filename, string out_filename){
	string data = read_file(data_filename); //The data for ciphering
	string key = read_file(key_filename); //The XOR key
	
	string cipher = my_xor(data, key);
	string hex_cipher = hexlify(cipher);
	
	ofstream out_file;
	out_file.open(out_filename);

	out_file << hex_cipher;

	out_file.close();
}

string read_file(string filename){
	string data;

	ifstream in_file;
	in_file.open(filename);
	
	getline(in_file, data);

	in_file.close();

	return data;
}

