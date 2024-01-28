		if(false)
		{
			int numE = 0;
			for (int t = 0; t < graph_size; t++)
			{
				numE += static_cast<int>(adj[t].size());
			}
			std::string file = __FUNCTION__;
			size_t index;
			for (index = 0; index < file.size(); index++)
			{
				if (file[index] == ':')
					break;
			}
			std::string fileName = file.substr(0, index) + ".txt";
			std::ofstream myfile;
			myfile.open(fileName.c_str(), std::ios::app);
			myfile << cycles[0] << " " << numE << "\n";
			myfile.close();
		}