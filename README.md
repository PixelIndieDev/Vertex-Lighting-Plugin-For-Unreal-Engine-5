# Vertex Lighting Plugin for Unreal Engine 5 ![UE5](https://img.shields.io/badge/Unreal%20Engine-5.5%2B-blue) ![GitHub all releases](https://img.shields.io/github/downloads/PixelIndieDev/Vertex-Lighting-Plugin-For-Unreal-Engine-5/total) 
> Easily integrate vertex lighting into your Unreal Engine projects with just a few clicks. This plugin is written almost entirely in Blueprint, offering a flexible and accessible solution for vertex-based lighting.

A lightweight **vertex lighting plugin** for **Unreal Engine 5 (UE5)** designed to deliver **per-vertex shading** light rendering.

This plugin is perfect for developers aiming to achieve:
- **Retro game visuals**
- **Stylized shading effects**
- **High performance real-time lighting for distant objects**

> [!TIP]
> [View the Documentation](https://pixelindiedev.github.io/Vertex-Lighting-Plugin-For-Unreal-Engine-5/)

> [!NOTE] 
> If you want more retro features like **N64 texture filtering**, **PSX vertex snapping** and **PSX like affine**, I suggest you to use **RetroFX** instead.
> You can download RetroFX by joining [**EvilReFlex's**](https://x.com/evilreflex) discord server.
> If you just need **vertex lighting**, **with some simple additions**, then this project is for you.

## How to Download and Install
**Download the Plugin**
- Get the plugin from the GitHub Releases page or by downloading the source code.
**Install the Plugin**
- Place the plugin folder into your project’s Plugins directory. (Create a Plugins folder if it doesn’t already exist.)
**Enable the Plugin**
- The plugin should be enabled by default. However, double-check that it is enabled in the Plugins menu within Unreal Engine.

![The Vertex Lighting Plugin being enabled](/docs/images/guide_images/plugin_enabled.avif)

## How to Use

1. **Set Up the Plugin**
   - In the Content Browser, navigate to the Vertex Lighting Plugin directory.
   - Drag and drop the `VertexLighting_Manager` into your level.
     <img src="docs/images/guide_images/manager_in_folder.avif" alt="A guide image for the VertexLighting_Manager location in the folder" width="700">

2. **Configure Materials**
   - Locate the `VertexLights_MaterialFunction` in the Logic folder within the Vertex Lighting Plugin directory.
     <img src="docs/images/guide_images/materialfunction_in_folder.avif" alt="A guide image for the materialfunction location in the folder" width="700">
   - Open the materials you want to use and multiply your base color/texture (Input A) with the `VertexLights_MaterialFunction` (Input B).
     <img src="docs/images/guide_images/materialfunction.avif" alt="A guide image for the materialfunction in a material" width="600">
   - Connect the result of the multiplication to the `Base Color` and/or `Emissive Color` of your material.

3. **Add Lights**
   - Place a `VertexLighting_Light` or `VertexLighting_Light_Animated` actor into your level to add lights.
     <img src="docs/images/guide_images/actors_in_folder.avif" alt="A guide image for the VertexLighting_Light and VertexLighting_Light_Animated location in the folder" width="700">
     
## Demo Level

The plugin includes a demo level showcasing multiple vertex lighting configurations. Experiment with the light settings to get a feel for their functionality and effects.
<img src="docs/images/guide_images/demo_map.avif" alt="The icon of the VertexLighting_Manager" width="1024">

## Credits
> [!NOTE] 
> Portions of this project are based and improved upon on the vertex lighting implementation originally created by [**EvilReFlex**](https://x.com/evilreflex), shared via Discord.
> 
> These parts are used under the terms of the [Creative Commons Attribution 4.0 International License (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/).
> 
> Modifications were made to adapt his code into a UE5 plugin and to extend its functionality into a way that fits my needs.
