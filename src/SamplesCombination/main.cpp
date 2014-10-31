#include <opencv2/opencv.hpp>
#include <iostream>

#include "FilesUtils.h"

using namespace std;
using namespace cv;

void PrintUsage()
{
	std::cout << "main -fg imagelist_foreground_dir -bg imagelist_background_dir -rm" << endl;
}

void MakeCombination( list<string> xml_files, string xml_filename, string dir )
{
	FileStorage storage( xml_filename, FileStorage::WRITE );

	storage << "images" << "[";
	for( auto if_file_name = xml_files.begin(); if_file_name != xml_files.end(); ++if_file_name )
	{
		if( (*if_file_name).find("_combined") != -1 )
			continue;
		FileStorage file_storage( dir + (*if_file_name), FileStorage::READ );
		FileNode images = file_storage["images"];
		
		for( FileNodeIterator it = images.begin(); it != images.end(); ++it )
		{
			string part_filename = string(*it);
			storage << ( string("\"") + part_filename + "\"" );
		}
	}
	storage << "]";
}

int main( int argc, char** argv )
{
	string fg_dir = "fg/", bg_dir = "bg/";

	for(int i=1; i<argc; ++i)
	{
		if( string( argv[i] ) == "-fg" )
		{
			++i;
			fg_dir = FilesUtils::fixDir( argv[i] );
		}
		else if( string( argv[i] ) == "-bg" )
		{
			++i;
			bg_dir = FilesUtils::fixDir( argv[i] );
		}
	}	

	list<string> bg_xml, fg_xml;
	fg_xml = FilesUtils::FilesInDir(fg_dir.c_str(), "*.xml");
	bg_xml = FilesUtils::FilesInDir(bg_dir.c_str(), "*.xml");

	MakeCombination(fg_xml, fg_dir + "fg_imlist_combined.xml", fg_dir );
	MakeCombination(bg_xml, bg_dir + "bg_imlist_combined.xml", bg_dir );
	return 0;
}