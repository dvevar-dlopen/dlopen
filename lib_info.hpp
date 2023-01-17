#ifndef _LIB_INFO_HPP_
#define _LIB_INFO_HPP_

//dlopen, dlerror, dlsym, dlclose
#include <dlfcn.h>

//C++17 filesystem
#include <fstream>
#include <iostream>
#include <filesystem>

#include <iostream>

namespace libinfo {

	using namespace std;

	class LibInfo {
		public:
			LibInfo();
			~LibInfo();
		
		private:
			string LIB_EXTENSION;
			string SYMBOL_NAME;
			
			bool libraryContainsSymbol(string libraryPath, string symbolName);
			static bool endsWith(string str, string suffix);

		public:
			void analyzeDir(string dirPath);
			void analyzeFile(string entryPath);

			virtual string toString();
	};	

}

#endif /* !_LIB_INFO_HPP_ */
