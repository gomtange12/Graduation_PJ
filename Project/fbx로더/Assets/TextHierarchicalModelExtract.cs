﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Windows;
using System.IO;

public class TextHierarchicalModelExtract : MonoBehaviour
{
    private StreamWriter textWriter = null;
    private int nFrames = 0;

    void WriteTabs(int nLevel)
    {
        for (int i = 0; i < nLevel; i++) textWriter.Write("\t");
    }

    void WriteString(int nLevel, string strToWrite)
    {
        WriteTabs(nLevel);
        textWriter.Write(strToWrite);
    }

    void WriteLineString(int nLevel, string strToWrite)
    {
        WriteTabs(nLevel);
        textWriter.WriteLine(strToWrite);
    }

    void WriteObjectName(int nLevel, string strHeader, Object obj)
    {
        WriteTabs(nLevel);
        textWriter.Write(strHeader + " ");
        textWriter.WriteLine((obj) ? string.Copy(obj.name).Replace(" ", "_") : "null");
    }

    void WriteVectors(int nLevel, string strHeader, Vector2[] vectors)
    {
        WriteString(nLevel, strHeader + " " + vectors.Length + " ");
        if (vectors.Length > 0)
        {
            foreach (Vector2 v in vectors) textWriter.Write(v.x + " " + v.y + " ");
        }
        textWriter.WriteLine(" ");
    }

    void WriteVectors(int nLevel, string strHeader, Vector3[] vectors)
    {
        WriteString(nLevel, strHeader + " " + vectors.Length + " ");
        if (vectors.Length > 0)
        {
            foreach (Vector3 v in vectors) textWriter.Write(v.x + " " + v.y + " " + v.z + " ");
        }
        textWriter.WriteLine(" ");
    }

    void WriteVectors(int nLevel, string strHeader, Vector4[] vectors)
    {
        WriteString(nLevel, strHeader + " " + vectors.Length + " ");
        if (vectors.Length > 0)
        {
            foreach (Vector4 v in vectors) textWriter.Write(v.x + " " + v.y + " " + v.z + " " + v.w + " ");
        }
        textWriter.WriteLine(" ");
    }

    void WriteColors(int nLevel, string strHeader, Color[] colors)
    {
        WriteString(nLevel, strHeader + " " + colors.Length + " ");
        if (colors.Length > 0)
        {
            foreach (Color c in colors) textWriter.Write(c.r + " " + c.g + " " + c.b + " " + c.a + " ");
        }
        textWriter.WriteLine(" ");
    }

    void WriteTextureCoords(int nLevel, string strHeader, Vector2[] uvs)
    {
        WriteString(nLevel, strHeader + " " + uvs.Length + " ");
        if (uvs.Length > 0)
        {
            foreach (Vector2 uv in uvs) textWriter.Write(uv.x + " " + (1.0f - uv.y) + " ");
        }
        textWriter.WriteLine(" ");
    }

    void WriteIntegers(int nLevel, string strHeader, int[] integers)
    {
        WriteString(nLevel, strHeader + " " + integers.Length + " ");
        if (integers.Length > 0)
        {
            foreach (int i in integers) textWriter.Write(i + " ");
        }
        textWriter.WriteLine(" ");
    }

    void WriteMatrix(Matrix4x4 matrix)
    {
        textWriter.Write(matrix.m00 + " " + matrix.m10 + " " + matrix.m20 + " " + matrix.m30 + " ");
        textWriter.Write(matrix.m01 + " " + matrix.m11 + " " + matrix.m21 + " " + matrix.m31 + " ");
        textWriter.Write(matrix.m02 + " " + matrix.m12 + " " + matrix.m22 + " " + matrix.m32 + " ");
        textWriter.Write(matrix.m03 + " " + matrix.m13 + " " + matrix.m23 + " " + matrix.m33 + " ");
    }

    void WriteLocalMatrix(int nLevel, string strHeader, Transform current)
    {
        WriteString(nLevel, strHeader + " ");
        Matrix4x4 matrix = Matrix4x4.identity;
        matrix.SetTRS(current.localPosition, current.localRotation, current.localScale);
        WriteMatrix(matrix);
        textWriter.WriteLine(" ");
    }

    void WriteWorldMatrix(int nLevel, string strHeader, Transform current)
    {
        WriteString(nLevel, strHeader + " ");
        Matrix4x4 matrix = Matrix4x4.identity;
        matrix.SetTRS(current.position, current.rotation, current.lossyScale);
        WriteMatrix(matrix);
        textWriter.WriteLine(" ");
    }

    int GetTexturesCount(Material[] materials)
    {
        int nTextures = 0;
        for (int i = 0; i < materials.Length; i++)
        {
            if (materials[i].GetTexture("_MainTex")) nTextures++;
            if (materials[i].GetTexture("_SpecGlossMap")) nTextures++;
            if (materials[i].GetTexture("_Metallic")) nTextures++;
            if (materials[i].GetTexture("_BumpMap")) nTextures++;
            if (materials[i].GetTexture("_EmissionMap")) nTextures++;
            if (materials[i].GetTexture("_DetailAlbedoMap")) nTextures++;
            if (materials[i].GetTexture("_DetailNormalMap")) nTextures++;
        }
        return (nTextures);
    }

    int GetTexturesCount(Transform current)
    {
        int nTextures = 0;
        Renderer meshRenderer = current.gameObject.GetComponent<Renderer>();
        if (meshRenderer) nTextures = GetTexturesCount(meshRenderer.materials);

        for (int k = 0; k < current.childCount; k++) nTextures += GetTexturesCount(current.GetChild(k));

        return (nTextures);
    }

    void WriteMeshInfo(int nLevel, Mesh mesh)
    {
        WriteObjectName(nLevel, "<Mesh>: " + mesh.vertexCount, mesh);

        WriteLineString(nLevel+1, "<Bounds>: " + mesh.bounds.center.x + " " + mesh.bounds.center.y + " " + mesh.bounds.center.z + " " + mesh.bounds.extents.x + " " + mesh.bounds.extents.y + " " + mesh.bounds.extents.z);

        WriteVectors(nLevel+1, "<Positions>:", mesh.vertices);
        WriteColors(nLevel+1, "<Colors>:", mesh.colors);
        WriteTextureCoords(nLevel+1, "<TextureCoords0>:", mesh.uv);
        WriteTextureCoords(nLevel+1, "<TextureCoords1>:", mesh.uv2);
        WriteVectors(nLevel+1, "<Normals>:", mesh.normals);

        if ((mesh.normals.Length > 0) && (mesh.tangents.Length > 0))
        {
            Vector3[] tangents = new Vector3[mesh.tangents.Length];
            Vector3[] binormals = new Vector3[mesh.tangents.Length];
            for (int i = 0; i < mesh.tangents.Length; i++)
            {
                tangents[i] = new Vector3(mesh.tangents[i].x, mesh.tangents[i].y, mesh.tangents[i].z);
                binormals[i] = Vector3.Normalize(Vector3.Cross(mesh.normals[i], tangents[i])) * mesh.tangents[i].w;
            }

            WriteVectors(nLevel+1, "<Tangents>:", tangents);
            WriteVectors(nLevel+1, "<BiNormals>:", binormals);
        }

        WriteIntegers(nLevel+1, "<Indices>:", mesh.triangles);

        WriteLineString(nLevel+1, "<SubMeshes>: " + mesh.subMeshCount);
        if (mesh.subMeshCount > 0)
        {
            for (int i = 0; i < mesh.subMeshCount; i++)
            {
                int[] subindicies = mesh.GetTriangles(i);
                WriteIntegers(nLevel+2, "<SubMesh>: " + i, subindicies);
            }
        }

        WriteLineString(nLevel, "</Mesh>");
    }

    void WriteMaterials(int nLevel, Material[] materials)
    {
        WriteLineString(nLevel, "<Materials>: " + materials.Length);
        if (materials.Length > 0)
        {
            for (int i = 0; i < materials.Length; i++)
            {
                WriteLineString(nLevel+1, "<Material>: " + i);

                Color albedo = materials[i].GetColor("_Color");
                WriteLineString(nLevel+2, "<AlbedoColor>: " + albedo.r + " " + albedo.g + " " + albedo.b + " " + albedo.a);
                Color emission = materials[i].GetColor("_EmissionColor");
                WriteLineString(nLevel+2, "<EmissiveColor>: " + emission.r + " " + emission.g + " " + emission.b + " " + emission.a);
                Color specular = materials[i].GetColor("_SpecColor");
                WriteLineString(nLevel+2, "<SpecularColor>: " + specular.r + " " + specular.g + " " + specular.b + " " + specular.a);

                WriteLineString(nLevel+2, "<Glossiness>: " + materials[i].GetFloat("_Glossiness"));
                WriteLineString(nLevel+2, "<Smoothness>: " + materials[i].GetFloat("_Smoothness"));
                WriteLineString(nLevel+2, "<Metallic>: " + materials[i].GetFloat("_Metallic"));
                WriteLineString(nLevel+2, "<SpecularHighlight>: " + materials[i].GetFloat("_SpecularHighlights"));
                WriteLineString(nLevel+2, "<GlossyReflection>: " + materials[i].GetFloat("_GlossyReflections"));

                Texture mainAlbedoMap = materials[i].GetTexture("_MainTex");
                WriteObjectName(nLevel+2, "<AlbedoMap>:", mainAlbedoMap);
                Texture specularcMap = materials[i].GetTexture("_SpecGlossMap");
                WriteObjectName(nLevel+2, "<SpecularMap>:", specularcMap);
                Texture metallicMap = materials[i].GetTexture("_Metallic");
                WriteObjectName(nLevel+2, "<MetallicMap>:", metallicMap);
                Texture bumpMap = materials[i].GetTexture("_BumpMap");
                WriteObjectName(nLevel+2, "<NormalMap>:", bumpMap);
                Texture emissionMap = materials[i].GetTexture("_EmissionMap");
                WriteObjectName(nLevel+2, "<EmissionMap>:", emissionMap);
                Texture detailAlbedoMap = materials[i].GetTexture("_DetailAlbedoMap");
                WriteObjectName(nLevel+2, "<DetailAlbedoMap>:", detailAlbedoMap);
                Texture detailNormalMap = materials[i].GetTexture("_DetailNormalMap");
                WriteObjectName(nLevel+2, "<DetailNormalMap>:", detailNormalMap);
            }
        }
        WriteLineString(nLevel, "</Materials>");
    }

    void WriteFrameInfo(int nLevel, Transform current)
    {
        int nTextures = GetTexturesCount(current);
        WriteObjectName(nLevel, "<Frame>: " + nFrames++ + " " + nTextures, current.gameObject);

        WriteString(nLevel+1, "<Transform>: ");
        textWriter.Write(current.localPosition.x + " " + current.localPosition.y + " " + current.localPosition.z + " ");
        textWriter.Write(current.localEulerAngles.x + " " + current.localEulerAngles.y + " " + current.localEulerAngles.z + " ");
        textWriter.Write(current.localScale.x + " " + current.localScale.y + " " + current.localScale.z + " ");
        textWriter.Write(current.localRotation.x + " " + current.localRotation.y + " " + current.localRotation.z + " " + current.localRotation.w + " ");
        textWriter.WriteLine(" ");

        WriteLocalMatrix(nLevel+1, "<TransformMatrix>:", current);
        //WriteWorldMatrix(nLevel+1, "<TransformMatrix>:", current);

        MeshFilter meshFilter = current.gameObject.GetComponent<MeshFilter>();
        if (meshFilter)
        {
            Mesh mesh = meshFilter.mesh;
            if (mesh) WriteMeshInfo(nLevel+1, mesh);
        }
        MeshRenderer meshRenderer = current.gameObject.GetComponent<MeshRenderer>();
        if (meshRenderer)
        { 
            Material[] materials = meshRenderer.materials;
            if (materials.Length > 0) WriteMaterials(nLevel+1, materials);
        }
        else
        {
            SkinnedMeshRenderer skinMeshRenderer = current.gameObject.GetComponent<SkinnedMeshRenderer>();
            if (skinMeshRenderer)
            {
                Mesh mesh = skinMeshRenderer.sharedMesh;
                if (mesh) WriteMeshInfo(nLevel+1, mesh);

                Material[] materials = skinMeshRenderer.materials;
                if (materials.Length > 0) WriteMaterials(nLevel+1, materials);
            }
        }
    }

    void WriteFrameHierarchyInfo(int nLevel, Transform child)
    {
        WriteFrameInfo(nLevel, child);

        WriteLineString(nLevel+1, "<Children>: " + child.childCount);

        if (child.childCount > 0)
        {
            for (int k = 0; k < child.childCount; k++) WriteFrameHierarchyInfo(nLevel+2, child.GetChild(k));
        }

        WriteLineString(nLevel, "</Frame>");
    }

    void Start()
    {
        textWriter = new StreamWriter(string.Copy(gameObject.name).Replace(" ", "_") + ".txt");

        WriteFrameHierarchyInfo(0, transform);
    
        textWriter.Flush();
        textWriter.Close();

        print("Mesh Write Completed");
    }
}

