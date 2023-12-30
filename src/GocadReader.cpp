#include "GocadReader.h"
#include "DirUtils.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <climits>
#include <random>
#include "glm/gtc/quaternion.hpp"


GocadReader::GocadReader()
{
    //ctor
	std::cout << "aGridLines\n";
	aGridLines = new DrawableLines();
	std::cout << "aSkeletonLines\n";
	aSkeletonLines = new DrawableLines();
	/*
	std::cout << "aCellLines\n";
	aCellLines = new DrawableLines();
	std::cout << "aCellSpheres\n";
	aCellSpheres = new DrawableSpheres();*/
	std::cout << "aEnvelopeTri\n";
	aEnvelopeTri = new DrawableTriangles();
	
}

GocadReader::~GocadReader()
{
    //dtor
}

void GocadReader::ReadDebugLogger(const std::string &filepath)
{
	if (!DirUtils::m_IsFile(filepath))
	{
		std::cout << "Debug file not found!\n";
		return;
	}
    std::ifstream file(filepath);
    std::cout<<file.is_open()<<std::endl;
    std::string line; 
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> indices;
    glm::dvec3 min_pt{ INT_MAX};
    glm::dvec3 max_pt{-INT_MAX};
    
    const glm::dvec3 grid_color(1.0, 0.0, 0.0);
	glm::dvec3 iline_color(0.0, 0.0, 0.0);
	glm::dvec3 envelope_color(0.0, 0.0, 0.0);
    glm::dvec3 v_origin{0.0}, v_side{0.0}, v_tangent{0.0}, v_up{0.0};
    
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    
    bool readGrid = false;
    bool readEnvelope = false;
    bool readSkeleton = false;
    bool readCells = false;

	bool read_tri_vertices = false;
	bool read_tri_indices = false;
	while (std::getline(file, line))
    {
		//std::cout<<"+++ "<<line<<"\n";
        if(line.find("### Cells end") != std::string::npos)
        {
            break;
        }
        if(line.find("### Grid start") != std::string::npos)
        {
            readGrid = true;
            readEnvelope = false;
            readSkeleton = false;
            readCells = false;
            continue;
        }
        if(line.find("### Envelope start") != std::string::npos)
        {
            readGrid = false;
            readEnvelope = true;
            readSkeleton = false;
            readCells = false;
            continue;
        }
        if(line.find("### Skeleton start") != std::string::npos)
        {
            readGrid = false;
            readEnvelope = false;
            readSkeleton = true;
            readCells = false;
            continue;
        }
        if(line.find("### Cells start") != std::string::npos)
        {
            readGrid = false;
            readEnvelope = false;
            readSkeleton = false;
            //readCells = true;
            continue;
        }
        
        
        if(readGrid)
        {
            if(line.length() == 0) 
            {
                continue;
            }
            if(line.find("# cell") != std::string::npos) 
            {
                //std::cout<<line<<std::endl;
                // add to drawable line
                if(points.size() > 0)
                {
                    for(size_t i = 0; i < points.size()-1; i++)
                    {
                        aGridLines->PushLine(points.at(i), points.at(i+1), grid_color);
                    }
                    points.clear();
                }
                continue;
            }
            if(line.find("### Grid end") != std::string::npos) 
            {
                readGrid = false;
                // add to drawable line
                //std::cout<<"points.size() "<<points.size()<<std::endl;
                for(size_t i = 0; i < points.size()-1; i++)
                {
                    aGridLines->PushLine(points.at(i), points.at(i+1), grid_color);
                }
                points.clear();
				std::cout << "Grid read\n";
                continue;
            }
            
			std::vector<std::string> tokenized;
			m_Tokenize(line, " ", tokenized);
			if(tokenized.size() < 12) 
			{
			    std::cout << "Error when reading grid cell points\n";
				std::cout << line << "\n";
			    tokenized.clear();
			    points.clear();
			    break;
			}
			//std::cout<<tokenized.at(0)<<" "<<tokenized.at(1)<<" "<<tokenized.at(2)<<std::endl;
			for(int i = 0; i < 9; i += 3)
			{
				glm::dvec3 pt{ std::stod(tokenized.at(i)), std::stod(tokenized.at(i + 1)), std::stod(tokenized.at(i + 2)) };

				min_pt.x = std::min(min_pt.x, pt.x);
			    min_pt.y = std::min(min_pt.y, pt.y);
			    min_pt.z = std::min(min_pt.z, pt.z);
			    
			    max_pt.x = std::max(max_pt.x, pt.x);
			    max_pt.y = std::max(max_pt.y, pt.y);
			    max_pt.z = std::max(max_pt.z, pt.z);
			    
    			points.push_back(pt);
			}
        }
        
     
        if(readSkeleton)
        {
            //std::cout<<line<<"\n";
            if(line.length() == 0 || line.find("# lines") != std::string::npos) 
            {
                continue;
            }
            if(line.find("# line ") != std::string::npos) 
            {
                //std::cout<<line<<std::endl;
                // add to drawable line
                if(points.size() > 0)
                {
                    iline_color.x = distribution(generator);
                    iline_color.y = distribution(generator);
                    iline_color.z = distribution(generator);
                    //std::cout<<"points.size() "<<points.size()<<std::endl;
                    for(size_t i = 0; i < points.size()-1; i++)
                    {
                        aSkeletonLines->PushLine(points.at(i), points.at(i+1), iline_color);
                    }
                    points.clear();
                }
                continue;
            }
            if(line.find("### Skeleton end") != std::string::npos) 
            {
                readSkeleton = false;
                // add to drawable line
                iline_color.x = distribution(generator);
                iline_color.y = distribution(generator);
                iline_color.z = distribution(generator);
                //std::cout<<"points.size() "<<points.size()<<std::endl;
                for(size_t i = 0; i < points.size()-1; i++)
                {
                    aSkeletonLines->PushLine(points.at(i), points.at(i+1), iline_color);
                }
                points.clear();
				std::cout << "Skeleton read\n";
                continue;
            }

			std::vector<std::string> tokenized;
			m_Tokenize(line, " ", tokenized);
			if(tokenized.size() < 6) 
			{
			    std::cout << "Error when reading skeleton points\n";
				std::cout << line << "\n";
			    tokenized.clear();
			    points.clear();
			    break;
			}
			//std::cout << "tokenized.size() " << tokenized.size() << std::endl;
			for(int i = 0; i < tokenized.size(); i += 3)
			{
				/*min_pt.x = std::min(min_pt.x, std::stod(tokenized.at(i)));
				min_pt.y = std::min(min_pt.y, std::stod(tokenized.at(i + 1)));
				min_pt.z = std::min(min_pt.z, std::stod(tokenized.at(i + 2)));

				max_pt.x = std::max(max_pt.x, std::stod(tokenized.at(i)));
				max_pt.y = std::max(max_pt.y, std::stod(tokenized.at(i + 1)));
				max_pt.z = std::max(max_pt.z, std::stod(tokenized.at(i + 2)));*/
			    glm::dvec3 pt{std::stod(tokenized.at(i)), std::stod(tokenized.at(i+1)), std::stod(tokenized.at(i+2))};
    			points.push_back(pt);
			}
        }
        
       
        if(readCells)
        {
			//std::cout<<"*** "<<line<<std::endl;
            if(line.length() == 0) 
            {
                continue;
            }
            if(line.find("### Cells end") != std::string::npos) 
            {
                readCells = false;
				std::cout << "Cells read\n";
                continue;
            }
            bool origin = line.find("Origin:") != std::string::npos;
            bool tangent = line.find("Tangent:") != std::string::npos;
            bool side = line.find("Side:") != std::string::npos;
            bool up = line.find("Up: ") != std::string::npos;
            bool in = line.find("In: ") != std::string::npos;
            bool out = line.find("Out: ") != std::string::npos;
			bool impactup = line.find("ImpactUpPt: ") != std::string::npos;
			bool impactdown = line.find("ImpactDownPt: ") != std::string::npos;
			bool impactleft = line.find("ImpactLeftPt:") != std::string::npos;
			bool impactright = line.find("ImpactRightPt:") != std::string::npos;
			bool maximpactup = line.find("MaxUpPt: ") != std::string::npos;
			bool maximpactdown = line.find("MaxDownPt: ") != std::string::npos;
			bool maximpactleft = line.find("MaxLeftPt:") != std::string::npos;
			bool maximpactright = line.find("MaxRightPt:") != std::string::npos;
            //if(line.find("p1:") != std::string::npos || line.find("p2:") != std::string::npos ||
            if(origin || tangent || side || up || in || out || impactup || impactdown || impactleft || impactright || maximpactup || maximpactdown || maximpactleft || maximpactright)
            {
		        std::vector<std::string> tokenized;
		        m_Tokenize(line, " ", tokenized);
		        if(tokenized.size() < 4) 
		        {
		            std::cout << "Error when reading cell points\n";
					std::cout << line << "\n";
		            tokenized.clear();
		            points.clear();
		            break;
		        }
		        
		        glm::dvec3 pt;
		        pt.x = std::stod(tokenized.at(1));
		        pt.y = std::stod(tokenized.at(2));
		        pt.z = std::stod(tokenized.at(3));
		        
		        if(origin)
		        {
		            v_origin = pt;
		            ///aCellSpheres->PushSphere(pt, glm::vec3(0.5, 0.0, 0.0), 0.5);
		            //std::cout<<"origin "<<pt.x<<" "<<pt.y<<" "<<pt.z<<"\n";
		        }
		        else if(in)
		        {
		            ///aCellSpheres->PushSphere(pt, glm::vec3(0.0, 0.0, 0.8), 0.5);
		            //std::cout<<"in "<<pt.x<<" "<<pt.y<<" "<<pt.z<<"\n";
		        }
				else if (out)
				{
					//aCellSpheres->PushSphere(pt, glm::vec3(0.0, 0.5, 0.0), 0.5);
					//std::cout << "out " << pt.x << " " << pt.y << " " << pt.z << "\n";
				}
				else if (maximpactleft || maximpactright)
				{
					//aCellSpheres->PushSphere(pt, glm::vec3(0.0, 0.0, 0.0), 0.5);
					//std::cout << "maximpact left/right " << pt.x << " " << pt.y << " " << pt.z << "\n";
				}
				else if (maximpactup || maximpactdown)
				{
					//aCellSpheres->PushSphere(pt, glm::vec3(0.0, 0.0, 0.0), 0.5);
					//std::cout << "maximpact up/down " << pt.x << " " << pt.y << " " << pt.z << "\n";
				}
				else if (impactup || impactdown)
				{
					//aCellSpheres->PushSphere(pt, glm::vec3(0.5, 0.5, 0.5), 0.5);
					//std::cout << "impact up/down " << pt.x << " " << pt.y << " " << pt.z << "\n";
				}
				else if (impactleft || impactright)
				{
					//aCellSpheres->PushSphere(pt, glm::vec3(0.5, 0.5, 0.5), 0.5);
					//std::cout << "impact left/right " << pt.x << " " << pt.y << " " << pt.z << "\n";
				}
		        else
		        {   
		            if(side) v_side = pt;
		            if(tangent) v_tangent = pt;
		            if(up)
		            {
		                v_up = pt;
		                double up_tang = glm::dot(v_up, v_tangent);
		                double up_side = glm::dot(v_up, v_side);
		                if(up_tang > 1e-6 || up_side > 1e-6)
		                {
		                    std::cout<<"up x t: "<<up_tang<<" up x s "<<up_side<<" ********************************\n";
                        }
		            }
		            pt = v_origin+pt*5.0;
		            ///aCellLines->PushLine(v_origin, pt, glm::vec3(0.8, 0.0, 0.0));
		            //std::cout<<"pt "<<pt.x<<" "<<pt.y<<" "<<pt.z<<"\n";
		        }
            }
       }
        

		if (readEnvelope)
		{
			//std::cout<<"*** "<<line<<std::endl;
			if (line.length() == 0)
			{
				continue;
			}
			if (line.find("# vertices") != std::string::npos)
			{
				read_tri_vertices = true;
				read_tri_indices = false;
				//std::cout<<line<<std::endl;
				// add to drawable line
				/*envelope_color.x = distribution(generator);
				envelope_color.y = distribution(generator);
				envelope_color.z = distribution(generator);
				if (points.size() > 0)
				{
					//std::cout<<"points.size() "<<points.size()<<std::endl;
					for (size_t i = 0; i < points.size() - 2; i = i + 3)
					{
						aEnvelopeTri->PushTriangle(points.at(i), points.at(i + 1), points.at(i + 2), envelope_color);
					}
					points.clear();
				}*/
				continue;
			}
			if (line.find("# indices") != std::string::npos)
			{
				read_tri_vertices = false;
				read_tri_indices = true;
				continue;
			}
			if (line.find("### Envelope end") != std::string::npos)
			{
				read_tri_vertices = false;
				read_tri_indices = false;
				readEnvelope = false;
				std::cout << "Envelope read\n";
				envelope_color.x = distribution(generator);
				envelope_color.y = distribution(generator);
				envelope_color.z = distribution(generator);

				aEnvelopeTri->SetBuffers(points, normals, envelope_color, indices);
				points.clear();
				indices.clear();
				normals.clear();
				//std::cout << "aEnvelopeTri->GetTotalTriangles() "<< aEnvelopeTri->GetTotalTriangles()<< "\n";
				continue;
			}

			std::vector<std::string> tokenized;
			m_Tokenize(line, " ", tokenized);
			if (tokenized.size() < 6)
			{
				std::cout << "Error when reading envelope points\n";
				std::cout << line << "\n";
				tokenized.clear();
				points.clear();
				break;
			}
			if (read_tri_indices)
			{
				for (int i = 0; i < tokenized.size(); i++)
				{
					indices.push_back(std::stoi(tokenized.at(i)));
					//std::cout << " " << std::stoi(tokenized.at(i));
					//if(i%3 == 2) std::cout << std::endl;
				}
				//std::cout << std::endl;
				tokenized.clear();
			}
			else if (read_tri_vertices)
			{
				for (int i = 0; i < 3; i += 3)
				{
					glm::dvec3 pt{ std::stod(tokenized.at(i)), std::stod(tokenized.at(i + 1)), std::stod(tokenized.at(i + 2)) };
					points.push_back(pt);
					//std::cout << "read_tri_vertices " << pt.x << " " << pt.y << " " << pt.z << "\n";
				}
				for (int i = 3; i < 6; i += 3)
				{
					glm::dvec3 pt{ std::stod(tokenized.at(i)), std::stod(tokenized.at(i + 1)), std::stod(tokenized.at(i + 2)) };
					normals.push_back(glm::normalize(pt));
					//std::cout << "read_tri_normals " << pt.x << " " << pt.y << " " << pt.z << "\n";
				}
				/*for (size_t i = 0; i < points.size() - 2; i = i + 3)
				{
					aEnvelopeTri->PushTriangle(points.at(i), points.at(i + 1), points.at(i + 2), normals.at(i), envelope_color);
				}
				points.clear();*/
				tokenized.clear();
			}

		}
        
    }
    file.close();
    
    //std::cout<<"min "<<std::to_string(min_pt.x)<<" "<<std::to_string(min_pt.y)<<" "<<std::to_string(min_pt.z)<<std::endl;
    //std::cout<<"max "<<std::to_string(max_pt.x)<<" "<<std::to_string(max_pt.y)<<" "<<std::to_string(max_pt.z)<<std::endl;
    glm::dvec3 center = (min_pt+max_pt)/2.0;
	std::cout<<"center "<<std::to_string(center.x)<<" "<<std::to_string(center.y)<<" "<<std::to_string(center.z)<<std::endl;
    
    glm::dmat4 t{1.0};
    t = glm::rotate(t, glm::radians(-90.0), glm::dvec3{1.0, 0.0, 0.0});
    glm::dmat3 rotation{
    t[0][0], t[0][1], t[0][2],
    t[1][0], t[1][1], t[1][2],
    t[2][0], t[2][1], t[2][2]};
    
    // Translating to a visible area
	for(int i = 0; i < aGridLines->GetTotalLines() ; i++)
    {
        aGridLines->RotateLinePosition(i, rotation, center);
        aGridLines->TranslateLinePosition(i, -center);
    }
	aGridLines->Update();
    for(int i = 0; i < aSkeletonLines->GetTotalLines() ; i++)
    {
        aSkeletonLines->RotateLinePosition(i, rotation, center);
        aSkeletonLines->TranslateLinePosition(i, -center);
    }
	aSkeletonLines->Update();
    /*
    for(int i = 0; i < aCellLines->GetTotalLines() ; i++)
    {
        aCellLines->RotateLinePosition(i, rotation, center);
        aCellLines->TranslateLinePosition(i, -center, -center);
    }
	for (int i = 0; i < aCellSpheres->GetTotalSpheres(); i++)
	{
		aCellSpheres->RotateSpherePosition(i, rotation, center);
		aCellSpheres->TranslateSpherePosition(i, -center);
	}*/
	/*for (int i = 0; i < aEnvelopeTri->GetTotalTriangles(); i++)
	{
		aEnvelopeTri->RotateTrianglePosition(i, rotation, center);
		aEnvelopeTri->TranslateTrianglePosition(i, -center);
	}
	aEnvelopeTri->Update();*/
	aEnvelopeTri->RotateTriangles(rotation, center);
	aEnvelopeTri->TranslateTriangles(-center);
	aEnvelopeTri->Update();
	std::cout << "done\n";
    
}

DrawableLines* GocadReader::GetSkeletonLines()
{
    return aSkeletonLines;
}

DrawableLines* GocadReader::GetGridLines()
{
    return aGridLines;
}

/*
DrawableLines* GocadReader::GetCellLines()
{
    return aCellLines;
}
DrawableSpheres* GocadReader::GetCellSpheres()
{
    return aCellSpheres;
}
*/
DrawableTriangles* GocadReader::GetEnvelopetriangles()
{
	return aEnvelopeTri;
}

void GocadReader::m_Tokenize(const std::string &text, const std::string &delimiters, std::vector<std::string> &tokenized)
	{
		tokenized.clear();
		if (delimiters.size() == 0) return;

		// [\\s,]+ means the white space and "," are delimiters

		std::string delimiters_{ delimiters };
		// replacing the white space with the symbol of white space in between []+
		size_t f = delimiters.find(" ");
		if (f != std::string::npos)
		{
			// replacing the content from/to with some string
			delimiters_.replace(f, f + 1, "\\s");
		}
		delimiters_ = "[" + delimiters_ + "]+";

		// setting the regular expression
		const std::regex ws_re(delimiters_);
		// std::copy do a thing for every element.
		// Don't worry, empty strings are not added
		std::copy(std::sregex_token_iterator(text.begin(), text.end(), ws_re, -1),
			std::sregex_token_iterator(),
			std::back_inserter(tokenized));
		/*
		const std::regex ws_re("\\s+"); // whitespace
		std::sregex_token_iterator tok_it = std::sregex_token_iterator(text.begin(), text.end(), ws_re, -1);
		while(tok_it != std::sregex_token_iterator())
		{
		//LOG(tok_it);
		std::cout<< *tok_it <<std::endl;
		tok_it++;
		}
		*/

		//const std::regex ws_re("\\s+"); // whitespace
		//std::copy( std::sregex_token_iterator(text.begin(), text.end(), ws_re, -1),
		//           std::sregex_token_iterator(),
		//           std::ostream_iterator<std::string>(std::cout, "\n"));
	}

