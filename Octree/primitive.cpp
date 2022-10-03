#include "octreegenerator.h"
#include "analysis.cpp"

voxel SuperVoxelLocal(){
	voxel supervoxel;
	supervoxel.height = 0;
	supervoxel.classe = 1;
	supervoxel.pai = NULL;

	return supervoxel;
}

void Generator::PrimEsfera() {
	// Supervoxel local
	octree octreeinstance;
	voxel supervoxel = SuperVoxelLocal();
	point c = point { 0, 2, 0 };
	float r = 2;
	supervoxel.len = 2 * r;
	supervoxel.point = { c.x - r, c.y - r, c.z - r };
	octreeinstance.vox.push_back(supervoxel);

	for (int i = 0; i < octreeinstance.vox.size(); i++) {
		// Percorre todos os cinza (inicialmente tds são cinzas)
		if (octreeinstance.vox[i].height < limit && octreeinstance.vox[i].classe == 1) {
			vector<voxel>* news = new vector<voxel>(8);
			// Dividide-os
			news = Partioning(octreeinstance.vox[i]);

			// Classifica
			for (int j = 0; j < 8; j++) {
				vector<point> coords = Voxel2Points(news->at(j));
				int count = 0;
				for (int k = 1; k < 8; k++) {
					float x = pow((coords[k].x - c.x), 2);
					float y = pow((coords[k].y - c.y), 2);
					float z = pow((coords[k].z - c.z), 2);
					float result = x + y + z;

					if (result <= (r * r))
						count++;
				}

				if (count == 0) {
					news->at(j).classe = 0;
				} else if (count == 7) {
					news->at(j).classe = 2;
				} else {
					news->at(j).classe = 1;
				}
			}
			// Insere na octree
			octreeinstance.vox[i].newvoxes = news;
			octreeinstance.vox.insert(octreeinstance.vox.end(), news->begin(), news->end());
		}
	}

	octrees.push_back(octreeinstance);
	// Analysis
	areaEsf(r);
	volEsf(r);
}
void Generator::PrimCilin() {
	// Supervoxel local
	octree octreeinstance;
	voxel supervoxel = SuperVoxelLocal();
	point c = point { 0, 2, 0 };
	float r = 1;
	float h = 5;
	point d = { 0, 1, 0 };
	d.norm();
	if (h > 2 * r) {
		supervoxel.len = h;
	} else {
		supervoxel.len = 2 * r;
	}
	supervoxel.point = { c.x - supervoxel.len/2, c.y, c.z - supervoxel.len/2 };
	octreeinstance.vox.push_back(supervoxel);

	// Percorre todos os cinza (inicialmente tds são cinzas)
	for (int i = 0; i < octreeinstance.vox.size(); i++) {
		if (octreeinstance.vox[i].height < limit && octreeinstance.vox[i].classe == 1) {
			vector<voxel>* news = new vector<voxel>(8);
			// Divide-os
			news = Partioning(octreeinstance.vox[i]);

			// Classifica
			for (int j = 0; j < 8; j++) {
				vector<point> coords = Voxel2Points(news->at(j));
				int count = 0;
				for (int k = 1; k < 8; k++) {
					const point v = point{
					coords[k].x - c.x,
					coords[k].y - c.y,
					coords[k].z - c.z };

					const double dir_dot_v = dot(d, v);
					const double norm_dir = d.length();
					const double norm_v = v.length();
					const double cos_tetha = dir_dot_v / (norm_dir * norm_v);
					const double sin_tetha = sqrt(1 - cos_tetha * cos_tetha);
					const double d_1 = norm_v * sin_tetha;

					if (d_1 <= r) 
						count++;
				}

				if (count == 0) {
					news->at(j).classe = 0;
				} else if (count == 7) {
					news->at(j).classe = 2;
				} else {
					news->at(j).classe = 1;
				}
			}

			// Insere na octree
			octreeinstance.vox[i].newvoxes = news;
			octreeinstance.vox.insert(octreeinstance.vox.end(), news->begin(), news->end());
		}
	}

	octrees.push_back(octreeinstance);
	// Analysis
	areaCilin(r, h);
	volCilin(r, h);
}
void Generator::PrimCube() {
	// Supervoxel local
	octree octreeinstance;
	voxel supervoxel = SuperVoxelLocal();
	point c = point{ 0.0f, 2.0f, 0.0f };
	float x = 2.0;
	float y = 2.0;
	float z = 2.0;
	supervoxel.len = x;
	supervoxel.point = c;
	octreeinstance.vox.push_back(supervoxel);

	// Percorre todos os cinza (inicialmente tds são cinzas)
	for (int i = 0; i < octreeinstance.vox.size(); i++) {
		if (octreeinstance.vox[i].height < limit && octreeinstance.vox[i].classe == 1) {
			vector<voxel>* news = new vector<voxel>(8);
			// Divide-os
			news = Partioning(octreeinstance.vox[i]);
			// Classifica
			for (int j = 0; j < 8; j++) {
				vector<point> coords = Voxel2Points(news->at(j));
				int count = 0;
				for (int k = 1; k < 8; k++) {
					bool inside = (coords[k].x <= c.x + x &&
						coords[k].x >= c.x - x &&
						coords[k].y <= c.y + y &&
						coords[k].y >= c.y - y &&
						coords[k].z <= c.z + z &&
						coords[k].z >= c.z - z);

					if (inside)
						count++;
				}

				if (count == 0) {
					news->at(j).classe = 0;
				}
				else if (count == 7) {
					news->at(j).classe = 2;
				}
				else {
					news->at(j).classe = 1;
				}
			}

			// Insere na octree
			octreeinstance.vox[i].newvoxes = news;
			octreeinstance.vox.insert(octreeinstance.vox.end(), news->begin(), news->end());
		}
	}
	octrees.push_back(octreeinstance);
	// Analysis
	areaCube(x, y, z);
	volCube(x, y, z);
}

void Generator::PrimCone() {
		// Supervoxel local
		octree octreeinstance;
		voxel supervoxel = SuperVoxelLocal();
		float r = 1.5;
		float h = 5;
		float cosTeta = h / (sqrtf(h*h + (r*r)));
		point d = point { 0, 1, 0 };
		point c = point{ 0, 2, 0 };
		if (h > 2 * r) 
			supervoxel.len = h;
		else 
			supervoxel.len = 2 * r;
		supervoxel.point = { c.x - supervoxel.len / 2, c.y, c.z - supervoxel.len / 2 };
		octreeinstance.vox.push_back(supervoxel);

		// Percorre todos os cinza (inicialmente tds são cinzas)
		for (int i = 0; i < octreeinstance.vox.size(); i++) {
			if (octreeinstance.vox[i].height < limit && octreeinstance.vox[i].classe == 1) {
				vector<voxel>* news = new vector<voxel>(8);
				// Divide-os
				news = Partioning(octreeinstance.vox[i]);
				// Classifica
				for (int j = 0; j < 8; j++) {
					vector<point> coords = Voxel2Points(news->at(j));
					int count = 0;
					for (int k = 1; k < 8; k++) {
						float cone_dist = dot(coords[k] - c, d);
						float cr = (cone_dist / h) * r;
						float orth_distance = (coords[k] - c).length() - cone_dist * d.length();

						if (orth_distance <= cr)
							count++;

					}

					if (count == 0) {
						news->at(j).classe = 0;
					}
					else if (count == 7) {
						news->at(j).classe = 2;
					}
					else {
						news->at(j).classe = 1;
					}
				}

				// Insere na octree
				octreeinstance.vox[i].newvoxes = news;
				octreeinstance.vox.insert(octreeinstance.vox.end(), news->begin(), news->end());
			}
		}

	octrees.push_back(octreeinstance);
	// Analysis
	areaCone(r, h);
	volCone(r, h);
}
