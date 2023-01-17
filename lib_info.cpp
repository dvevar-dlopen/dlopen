#include "lib_info.hpp"

#define VERBOSE

#ifdef VERBOSE
		bool IS_VERBOSE = true;
#endif

#ifdef VERBOSE
#define LOG(...) if(IS_VERBOSE) { printf(__VA_ARGS__); /*Flush so that everyhing gets logged even in case of Segfault:*/fflush(stdout);}
#else
#define LOG(...) //
#endif

#define PRINTF(...) printf(__VA_ARGS__); /*Flush so that everyhing gets logged even in case of Segfault:*/fflush(stdout);

//#include <iostream>

namespace fs = std::filesystem;

namespace libinfo {

	using namespace std;

	///////////////////////////////////////////////////////////////////////////

	/**************************************************************************
	 *
	 */
	LibInfo::LibInfo() {
		LIB_EXTENSION = ".so";

		//E.g.: should be found in /usr/lib/x86_64-linux-gnu/xtables/libxt_cgroup.so
		SYMBOL_NAME = "libxt_cgroup_init";
	}

	/**************************************************************************
	 *
	 */
	LibInfo::~LibInfo() {
	}

	/**************************************************************************
	 *
	 */
	bool LibInfo::libraryContainsSymbol(string libraryPath, string symbolName) {
		char *errorDescription = NULL;

		void *handle = dlopen((char*)libraryPath.c_str(), RTLD_LAZY); //Sometimes causes Segmentation fault!?!
		if (!handle) {
			LOG("\tdlopen failure: handle == NULL: %s | %s\n", libraryPath.c_str(), symbolName.c_str());
			return false;
		}
		errorDescription = dlerror();
		if (errorDescription != NULL)  {
			LOG("\tdlopen error: %s\n", errorDescription);
			return false;
		}

		dlsym(handle, symbolName.c_str());
		errorDescription = dlerror();
		if (errorDescription != NULL)  {
			LOG("\tdlsym error: %s\n", errorDescription);
			dlclose(handle);
			return false;
		}

		dlclose(handle);

		return true;
	}

	/**************************************************************************
	 *
	 */
	bool LibInfo::endsWith(string str, string suffix) {
		if (str.length() < suffix.length()) {
			return false;
		}
		return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
	}

	/**************************************************************************
	 *
	 */
	void LibInfo::analyzeDir(string dirPath) {
		LOG("Looking for shared (dynamic) library containing symbol %s in %s and below.\n", SYMBOL_NAME.c_str(), dirPath.c_str());

		try {
			fs::recursive_directory_iterator iterator(
				dirPath,
				fs::directory_options::skip_permission_denied
			);
			for(const fs::directory_entry& dir_entry : iterator) {
				if(
					dir_entry.is_block_file()
					|| dir_entry.is_character_file()
					|| dir_entry.is_fifo()
					|| dir_entry.is_other()
					|| dir_entry.is_socket()
					|| dir_entry.is_symlink()
				) {
					continue;
				}
				if(dir_entry.is_regular_file()) {
					string entryPath = dir_entry.path();
					//Checking for extension .so is not adequate as it results in segfault in some cases.
					//Is there any other (fast and general) way to identify libraries so that checking them would not cause segfaults?
					if(endsWith(entryPath, LIB_EXTENSION)) {
						LOG("Checking %s\n", entryPath.c_str());
						if(libraryContainsSymbol(entryPath, SYMBOL_NAME)) {
							PRINTF("\tFound %s starting in %s.\n", entryPath.c_str(), dirPath.c_str());
						}
					}
				}
			}

			LOG("Done.\n");
		}
		catch(fs::filesystem_error const& e) {
			LOG("Caught error (code=%u, message: %s): %s\n\tpath1: %s\n\tpath2: %s\n", e.code().value(), e.code().message().c_str(), e.what(), e.path1().c_str(), e.path2().c_str());
		}
	}

	/**************************************************************************
	 *
	 */
	void LibInfo::analyzeFile(string entryPath) {
		LOG("Checking library: %s\n", entryPath.c_str());
		if(libraryContainsSymbol(entryPath, SYMBOL_NAME)) {
			LOG("Library %s contains %s.\n", entryPath.c_str(), SYMBOL_NAME.c_str());
		} else {
			LOG("Library %s doesn't contain %s.\n", entryPath.c_str(), SYMBOL_NAME.c_str());
		}
	}

	/**************************************************************************
	 *
	 */
	string LibInfo::toString() {
		ostringstream os(ostringstream::out);
		os << "Looking for " << SYMBOL_NAME << ".";
		os.flush();
		return os.str();
	}
}

///////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace libinfo;

/******************************************************************************
 *
 */
void cppInfo() {
	std::cout << "__cplusplus = " << __cplusplus << ": ";
	if (__cplusplus == 201703L) std::cout << "C++17\n";
	else if (__cplusplus == 201402L) std::cout << "C++14\n";
	else if (__cplusplus == 201103L) std::cout << "C++11\n";
	else if (__cplusplus == 199711L) std::cout << "C++98\n";
	else std::cout << "pre-standard C++\n";
}

/******************************************************************************
 *
 */
int main(int argc, const char**argv) {
	cppInfo();


	LibInfo lib;
	cout << lib.toString() << endl;

	lib.analyzeDir("/usr/lib");
	//Checking /usr/lib/x86_64-linux-gnu/libirs-9.18.1-1ubuntu1.2-Ubuntu.so
	//tls.c:83: fatal error: RUNTIME_CHECK(OPENSSL_init_ssl((0x00000200L | 0x00000400L | 0x00001000L | 0x00002000L | 0x00004000L) | 0x00000040L, ((void *)0)) == 1) failed
	//Aborted (core dumped)


	//lib.analyzeFile("/usr/lib/x86_64-linux-gnu/xtables/libxt_NFQUEUE.so");
	//Checking library: /usr/lib/x86_64-linux-gnu/xtables/libxt_NFQUEUE.so
	//Segmentation fault (core dumped)

	return 0;
}
