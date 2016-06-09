using UnityEngine;
using System.Collections;

public class blocks : MonoBehaviour {

    public Texture2D image;
    public Texture2D image2;
    public Texture2D image3;
    public int height = 1;
    public int wallHeight = 3;
    //public int levelHeight = 6;
    public bool terrainGen = true;
	// Use this for initialization
	void Start () {
        levelGen(image, 0);
        levelGen(image2, 1);
        levelGen(image3, 2);
	}

    void levelGen(Texture2D map, int level)
    {
        int levelHeight = level * 8;
        Color[] pixels = map.GetPixels(0, 0, image.width, image.height);
        float depth = 1f;
        GameObject obj;
        Renderer rend;
        for (int x = 0; x < map.width; x++)
        {
            for (int y = 0; y < map.height; y++)
            {
                Color color = pixels[(x * map.width) + y];
                //GameObject obj = GameObject.CreatePrimitive(PrimitiveType.Cube);
                //GameObject obj = GenerateCube(x, 0, y, (color.grayscale * height) + depth); // create cube set height view grayscale

                if (color == new Color(1f, 0f, 0f))
                {
                    if (!terrainGen)
                    {
                        obj = GenerateCube(x, levelHeight, y, wallHeight + depth);// create cube set height via color
                        obj.transform.position = new Vector3(x, wallHeight, y);
                    }
                    else
                    {
                        obj = GenerateCube(x, levelHeight, y, wallHeight * 3);// create cube set height via color
                        obj.transform.position = new Vector3(x, wallHeight * 3, y);
                    }
                    //rend = obj.GetComponent<Renderer>();
                    //rend.material.color = new Color(1, 1, 1);

					obj.gameObject.GetComponent<MeshRenderer>().material = Resources.Load ("Wall") as Material;
                }
                else if (terrainGen)
                {
                    obj = GenerateCube(x, levelHeight, y, (color.grayscale * height) + depth); // create cube set height view grayscale
                    obj.transform.position = new Vector3(x, color.grayscale * height, y);
                    rend = obj.GetComponent<Renderer>();
                    if (color.grayscale <= 0.20f)
                    {
                        color = new Color(rgb(55), rgb(155), rgb(255));
						obj.gameObject.GetComponent<MeshRenderer>().material = Resources.Load ("Water") as Material;;
                    }
                    else if (color.grayscale <= 0.3f)
                    {
                        color = new Color(rgb(255), rgb(230), rgb(128));
						obj.gameObject.GetComponent<MeshRenderer>().material = Resources.Load ("Sand") as Material;
                    }
                    else if (color.grayscale <= 0.6f)
                    {
                        color = new Color(rgb(58), rgb(146), rgb(56));
						obj.gameObject.GetComponent<MeshRenderer>().material = Resources.Load ("Grass") as Material;
                    }
                    else if (color.grayscale <= 0.8f)
                    {
                        color = new Color(rgb(116), rgb(116), rgb(116));
						obj.gameObject.GetComponent<MeshRenderer>().material = Resources.Load ("Rock") as Material;
                    }
                    else
                    {
                        color = new Color(rgb(218), rgb(218), rgb(218));
						obj.gameObject.GetComponent<MeshRenderer>().material = Resources.Load ("Snow") as Material;
                    }
                    rend.material.color = color;
                }
                else if(color != new Color(0f, 0f, 1f))
                {
                    obj = GenerateCube(x, levelHeight, y, depth);// create cube set height via color
                    obj.transform.position = new Vector3(x, 0, y);
                    //rend = obj.GetComponent<Renderer>();
                    //rend.material.color = new Color(0.1f, 0.1f, 0.1f);
					obj.gameObject.GetComponent<MeshRenderer>().material = Resources.Load ("Floor") as Material;
                }

                if (color == new Color(1f, 1f, 0f))
                {
                    GameObject light = new GameObject("Light");
                    light.gameObject.tag = "Untagged";
                    Light lit = light.AddComponent<Light>();
                    lit.range = 100f;
                    lit.type = LightType.Point;
                    lit.transform.position = new Vector3(x*2, levelHeight + 4, y*2);
                    //light.transform.position = new Vector3(x, 12, y);
                    //obj.transform.position = new Vector3(x, 4, y);
                }

                //rend.material.color = new Color(0.5f,0.5f,0.5f);
            }
        }
    }

    float rgb(float val)
    {
        return (val / 256);
    }
	
	// Update is called once per frame
	void Update () {
    }

    public GameObject GenerateCube(float x, float y, float z, float height = 1.0f)
    {
        float length = 1.0f;
        float width = 1.0f;

        float scale = 1.0f;

        //y += height/2;
            //length += 10;
            GameObject cube = new GameObject();
            cube.gameObject.tag = "Untagged";
            cube.gameObject.name = "Cube Object";
            //filter = gameObject.AddComponent<MeshFilter>();
            //mesh = filter.mesh;
            MeshFilter filter = cube.AddComponent<MeshFilter>();
            cube.AddComponent<MeshRenderer>();
            //cube.AddComponent<BoxCollider>();
            //Renderer rend = cube.GetComponent<Renderer>();
            //rend.material = grassMat;
            Mesh mesh = filter.mesh;

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
        return cube;
    }
}
