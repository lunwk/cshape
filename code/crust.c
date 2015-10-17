#include "crust.h"
#include "shape.h"

/*-------------------------------------------------------------------*/
//headers from qhull
#include "qhull.h"
#include "poly.h"
#include "qset.h"
#include "geom.h"

/*-------------------------------------------------------------------*/
//defined in shape.h/.c
extern tVertex vertices;
extern tEdge edges;
extern tFace faces;
extern tTetra tetras;

/*-------------------------------------------------------------------*/
void	Crust( void )
{
	tVertex ptr_v;
	tVertex *all_v = NULL;
	int vsize = 0;
	int id = 0;

	//static char* options = (char*)"delaunay QJ Pp";
	static char* options = (char*)"voronoi QJ Pp";
	coordT *pt = NULL;
	int curlong, totlong;
	facetT *facet = NULL;
	vertexT *vertex = NULL;
	vertexT **vertexp = NULL;
	tList voronoiVertex = NULL;
	tVertex center = NULL;
	
	//Count the number of points
	ptr_v = vertices;
	do {
		vsize++;
		ptr_v = ptr_v->next;
	} while (ptr_v != vertices);

	//Allocate memory
	pt = (coordT*)calloc(vsize * 4, sizeof(coordT));
	all_v = (tVertex*)calloc(vsize, sizeof(tVertex));
	assert(pt && all_v);

	//Copy points 
	ptr_v = vertices;
	do {
		pt[id++] = ptr_v->v[0];
		pt[id++] = ptr_v->v[1];
		pt[id++] = ptr_v->v[2];
		pt[id++] = ptr_v->v[0] * ptr_v->v[0] + ptr_v->v[1] * ptr_v->v[1] + ptr_v->v[2] * ptr_v->v[2];
		all_v[ptr_v->vnum] = ptr_v;
		ptr_v = ptr_v->next;
	} while (ptr_v != vertices);

	//
	// compute convex hull in 3D by calling qhull
	//

	qh_init_A(stdin, stdout, stderr, 0, NULL);
	qh_initflags(options);
	qh_init_B(pt, vsize, 4, false);
	qh_qhull();
	qh_check_output();

	qh_setvoronoi_all();

	//loop through all faces
	FORALLfacets
	{
		//get the vertex of voronoi diagram
		//loop through each vertex
		FOREACHvertex_(facet->vertices)
		{
			//copy the center of delaunay triangle
			NEW(center, tsVertex);
			center->v[0] = facet->center[0]; 
			center->v[1] = facet->center[1];
			center->v[2] = facet->center[2];
			center->next = center->prev = NULL;
			center->ispole = false;
			center->vvlist = NULL;
		
			//the vertex of vornoi diagram
			NEW(voronoiVertex, tsList);		//�� ������ ������ �Ҵ� ��������???
			voronoiVertex->p = (void*)center;
			voronoiVertex->next = voronoiVertex->prev = NULL;

			ADD(all_v[qh_pointid(vertex->point)]->vvlist, voronoiVertex);	// the center of delanauy triangle = the vertex of voronoi diagram 
		}
	}

	//Compute Pole



	free(pt);
	free(all_v);
	all_v = NULL;
	voronoiVertex = NULL;
	qh_freeqhull(!qh_ALL);
	qh_memfreeshort(&curlong, &totlong);
}

//Find a pole

//Find an antipole


//if the normal vector of tetrahedron points downward(=lower convex hull) and the volume is not zero, 
//generate faces. (I didn't care about duplications of faces)


