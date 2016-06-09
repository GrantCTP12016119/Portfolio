using UnityEngine;
using System.Collections;

public class gameControllerScript : MonoBehaviour {

    // You can change that line to provide another MeshFilter
    public GameObject cube;
    public MeshFilter filter;
    public Mesh mesh;
    public Renderer rend;

    public static readonly Color black = new Color(0f, 0f, 0f, 0f);
    public static readonly Color white = new Color(1f, 1f, 1f, 1f);

    public Texture2D sourceTex;
    public Rect sourceRect;
    public Texture2D destTex;
    public Color[] pix;

    public Material grassMat;

    float length = 1f;
    float width = 1f;
    float height = 1f;

    float scale = .5f;

    public void Start()
    {
        //pos = 0;
        //Cube(pos);
        for (int i = 0; i < 256; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                //pix[(i*256)+j] = sourceTex.GetPixel(i, j);
                Color color = sourceTex.GetPixel(i, j);
                print(1);
            }
        }
        //getpixels
         //int x = Mathf.FloorToInt(sourceRect.x);
         //int y = Mathf.FloorToInt(sourceRect.x);
        // int width = Mathf.FloorToInt(sourceRect.width);
        //print(sourceRect.width);
        // int height = Mathf.FloorToInt(sourceRect.height);
         //pix = sourceTex.GetPixels(x, y, width, height);
        //destTex = new Texture2D(width, height);
        //destTex.SetPixels(pix);
        //destTex.Apply();
        //GetComponent<Renderer>().material.mainTexture = destTex;

        //generate cubes
        //int pos = 0;
        //CubeGeneration(0, 0, 0);
    }

    // Use this for initialization
    public void CubeGeneration(int x, int y, int z)
    {
        //print(pix[128]);
        //pix[1] = black;
        float length = sourceRect.width + sourceRect.height;
        int i = 0;
        while (i < length)
        {
            print(pix[i]);
            //height += .01f;
            //if (pix[i] == black)
            if(i%4 == 0)
            {
                height = 10.0f;
            }
            else
            {
                height = 1.0f;
            }
            //height = (1 - Mathf.Cos(height * Mathf.PI)) / 2;
            if (x >= length/100)
            {
                x = 0;
                z += 3;
            }
            //length += 10;
            cube = new GameObject();
            cube.gameObject.tag = "Untagged";
            //filter = gameObject.AddComponent<MeshFilter>();
            //mesh = filter.mesh;
            filter = cube.AddComponent<MeshFilter>();
            cube.AddComponent<MeshRenderer>();
            rend = cube.GetComponent<Renderer>();
            rend.material = grassMat;
            mesh = filter.mesh;

            //mesh.Clear();

            //for (int i = 0; i < 2; i += 1)
            //int i = 0;
            {
                #region Vertices
                Vector3 p0 = new Vector3((x - length) * scale, ((y - height) * scale), (z + width) * scale);
                Vector3 p1 = new Vector3((x + length) * scale, ((y - height) * scale), (z + width) * scale);
                Vector3 p2 = new Vector3((x + length) * scale, ((y - height) * scale), (z - width) * scale);
                Vector3 p3 = new Vector3((x - length) * scale, ((y - height) * scale), (z - width) * scale);

                Vector3 p4 = new Vector3((x - length) * scale, ((y + height) * scale), (z + width) * scale);
                Vector3 p5 = new Vector3((x + length) * scale, ((y + height) * scale), (z + width) * scale);
                Vector3 p6 = new Vector3((x + length) * scale, ((y + height) * scale), (z - width) * scale);
                Vector3 p7 = new Vector3((x - length) * scale, ((y + height) * scale), (z - width) * scale);

                Vector3[] vertices = new Vector3[]
                {
	// Bottom
	p0, p1, p2, p3,
 
	// Left
	p7, p4, p0, p3,
 
	// Front
	p4, p5, p1, p0,
 
	// Back
	p6, p7, p3, p2,
 
	// Right
	p5, p6, p2, p1,
 
	// Top
	p7, p6, p5, p4
                };
                #endregion

                #region Normales
                Vector3 up = Vector3.up;
                Vector3 down = Vector3.down;
                Vector3 front = Vector3.forward;
                Vector3 back = Vector3.back;
                Vector3 left = Vector3.left;
                Vector3 right = Vector3.right;

                Vector3[] normales = new Vector3[]
                {
	// Bottom
	down, down, down, down,
 
	// Left
	left, left, left, left,
 
	// Front
	front, front, front, front,
 
	// Back
	back, back, back, back,
 
	// Right
	right, right, right, right,
 
	// Top
	up, up, up, up
                };
                #endregion

                #region UVs
                Vector2 _00 = new Vector2(0f, 0f);
                Vector2 _10 = new Vector2(1f, 0f);
                Vector2 _01 = new Vector2(0f, 1f);
                Vector2 _11 = new Vector2(1f, 1f);

                Vector2[] uvs = new Vector2[]
                {
	// Bottom
	_11, _01, _00, _10,
 
	// Left
	_11, _01, _00, _10,
 
	// Front
	_11, _01, _00, _10,
 
	// Back
	_11, _01, _00, _10,
 
	// Right
	_11, _01, _00, _10,
 
	// Top
	_11, _01, _00, _10,
                };
                #endregion

                #region Triangles
                int[] triangles = new int[]
                {
	// Bottom
	3, 1, 0,
    3, 2, 1,			
 
	// Left
	3 + 4 * 1, 1 + 4 * 1, 0 + 4 * 1,
    3 + 4 * 1, 2 + 4 * 1, 1 + 4 * 1,
 
	// Front
	3 + 4 * 2, 1 + 4 * 2, 0 + 4 * 2,
    3 + 4 * 2, 2 + 4 * 2, 1 + 4 * 2,
 
	// Back
	3 + 4 * 3, 1 + 4 * 3, 0 + 4 * 3,
    3 + 4 * 3, 2 + 4 * 3, 1 + 4 * 3,
 
	// Right
	3 + 4 * 4, 1 + 4 * 4, 0 + 4 * 4,
    3 + 4 * 4, 2 + 4 * 4, 1 + 4 * 4,
 
	// Top
	3 + 4 * 5, 1 + 4 * 5, 0 + 4 * 5,
    3 + 4 * 5, 2 + 4 * 5, 1 + 4 * 5,

                };
                #endregion

                mesh.vertices = vertices;
                mesh.normals = normales;
                mesh.uv = uvs;
                mesh.triangles = triangles;

                mesh.RecalculateNormals();
                mesh.RecalculateBounds();
                mesh.Optimize();

            }
            i++;
            x += 3;
        }
    }
	// Update is called once per frame
	void Update () {
    }
}
