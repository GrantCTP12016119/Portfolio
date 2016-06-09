using UnityEngine;
using System.Collections;

[RequireComponent(typeof(MeshCollider))]
[RequireComponent(typeof(MeshFilter))]
[RequireComponent(typeof(MeshRenderer))]
public class cubeScript : MonoBehaviour
{

    public float cubeX = 1f;
    public float cubeY = 1f;
    public float cubeZ = 1f;
    public float sections = 1f;

    void Start()
    {
        MeshFilter meshFilter = GetComponent<MeshFilter>();

        // FACE 1 VERTS
        Vector3 p0 = new Vector3(0, 0, 0);
        Vector3 p1 = new Vector3(cubeX, 0, 0);
        Vector3 p2 = new Vector3(cubeX, cubeY, 0);
        Vector3 p3 = new Vector3(0, cubeY, 0);

        // FACE 2 VERTS
        Vector3 p4 = new Vector3(cubeX, 0, 0);
        Vector3 p5 = new Vector3(cubeX, 0, cubeZ);
        Vector3 p6 = new Vector3(cubeX, cubeY, cubeZ);
        Vector3 p7 = new Vector3(cubeX, cubeY, 0);

        Mesh mesh = meshFilter.sharedMesh;

        if (mesh == null)
        {
            meshFilter.mesh = new Mesh();
            mesh = meshFilter.sharedMesh;
        }

        mesh.Clear();

        // Returns a copy of the vertex positions or assigns a new vertex positions array.
        mesh.vertices = new Vector3[]{
             p0,p1,p2,
             p0,p2,p3,
             p2,p1,p3,
             p0,p3,p1,

             p4,p5,p6,
             p4,p6,p5,
             p4,p6,p7,
             p4,p7,p6

         };

        // Array containing all triangles in the mesh.
        mesh.triangles = new int[]{
             0,1,2,
             3,4,5,
             6,7,8,
             9,10,11,

             12,13,14,
             15,16,17,
             18,19,20,
             21,22,23
         };

        mesh.RecalculateNormals();
        mesh.RecalculateBounds();
        mesh.Optimize();
    }
}