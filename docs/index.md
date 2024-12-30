<img src="https://pixelindiedev.github.io/docs/images/plugin_icon/VertexLights_Plugin_Icon.png" alt="The icon of the Vertex Lighting Plugin" width="250">

## How to Download and Install

1. **Download the Plugin**
   - Get the plugin from the GitHub Releases page or by downloading the source code.

2. **Install the Plugin**
   - Place the plugin folder into your project's `Plugins` directory. (Create a `Plugins` folder if it doesn't already exist.)

3. **Enable the Plugin**
   - The plugin should be enabled by default. However, double-check that it is enabled in the **Plugins** menu within Unreal Engine. <img src="/docs/images/guide_images/plugin_enabled.png" alt="The Vertex Lighting Plugin being enabled" width="400">

---

## How to Use

1. **Set Up the Plugin**
   - In the Content Browser, navigate to the Vertex Lighting Plugin directory.
   - Drag and drop the `VertexLighting_Manager` into your level. <img src="https://pixelindiedev.github.io/docs/images/guide_images/manager_in_folder.png" alt="A guide image for the VertexLighting_Manager location in the folder" width="700">

2. **Configure Materials**
   - Locate the `VertexLights_MaterialFunction` in the Logic folder within the Vertex Lighting Plugin directory. <img src="https://pixelindiedev.github.io/docs/images/guide_images/materialfunction_in_folder.png" alt="A guide image for the materialfunction location in the folder" width="700">
   - Open the materials you want to use and multiply your base color/texture (Input A) with the `VertexLights_MaterialFunction` (Input B). <img src="https://pixelindiedev.github.io/docs/images/guide_images/materialfunction.png" alt="A guide image for the materialfunction in a material" width="400">
   - Connect the result of the multiplication to the `Base Color` and/or `Emissive Color` of your material.

3. **Add Lights**
   - Place a `VertexLighting_Light` or `VertexLighting_Light_Animated` actor into your level to add lights. <img src="https://pixelindiedev.github.io/docs/images/guide_images/actors_in_folder.png" alt="A guide image for the VertexLighting_Light and VertexLighting_Light_Animated location in the folder" width="700">

---

## Demo Level

The plugin includes a demo level showcasing multiple vertex lighting configurations. Experiment with the light settings to get a feel for their functionality and effects.

---

## Plugin Components

### **VertexLighting_Manager**
The `VertexLighting_Manager` is essential for vertex lighting functionality. Ensure only one manager is present per level.
<img src="https://pixelindiedev.github.io/docs/images/actor_icons/Light_Manager.png" alt="The icon of the VertexLighting_Manager" width="200">

#### Default Settings:
- **Ambient Color**: Default environmental color.
  - *Default*: Hex sRGB - 7C7C7CFF | *Type*: Linear Color
- **SunNormalInfluence**: Controls how directional the vertex light sunlight should be, on a percentage basis. 0% makes the sunlight completely non-directional, while 100% makes it fully directional.
  - *Default*: 0% | *Type*: Float | *Range*: 0.0-100.0
- **ShowCombinedVertexLightInfo**: Enables debug information display.
  - *Default*: True | *Type*: Boolean

#### Tick Rate:
- **Light Update Tickrate**: Time interval between light updates. Set to `0.0` to disable updates.
  - *Default*: 0.1s | *Type*: Float | *Range*: 0.0-Infinite

#### In-Editor Events:
- **Draw All Preview Lights**: Displays all vertex lights in the editor.
- **Unregister All Preview Lights**: Stops displaying vertex lights in the editor.

---

### **VertexLighting_Light**
The `VertexLighting_Light` adds static or dynamic vertex lights to your scene.   
<img src="https://pixelindiedev.github.io/docs/images/actor_icons/Light.png" alt="The icon of the VertexLighting_Light" width="200">

#### Default Settings:
- **Light Color**: Color of the light. Overridden by color curves for animated lights.
  - *Default*: Hex sRGB - FFFFFFFF | *Type*: Linear Color
- **Brightness**: Light intensity. Overridden by color curves for animated lights.
  - *Default*: 10x | *Type*: Float | *Range*: 0.0-100.0
- **Influence Radius**: Radius of the light's influence.
  - *Default*: 300cm | *Type*: Float | *Range*: 0.0-Infinite
- **Influence to Normal**: Controls how directional the vertex light should be, on a percentage basis. 0% makes the light completely non-directional, while 100% makes it fully directional.
  - *Default*: 0% | *Type*: Float | *Range*: 0.0-100.0
- **Render Distance**: Maximum render distance for the light.
  - *Default*: 10,000cm | *Type*: Float | *Range*: 0.0-10,000.0
- **Is Movable Light**: Makes the light's position dynamic during updates.
  - *Default*: False | *Type*: Boolean
- **Should Turn On In Range**: Determines if the light activates when the player is nearby.
  - *Default*: True | *Type*: Boolean

#### Editor Settings:
- **Show Render Distance**: Displays the render distance sphere in the editor.
  - *Default*: False | *Type*: Boolean

---

### **VertexLighting_Light_Animated**
The `VertexLighting_Light_Animated` inherits from `VertexLighting_Light`.   
<img src="https://pixelindiedev.github.io/docs/images/actor_icons/Light_Animated.png" alt="The icon of the VertexLighting_Light_Animated" width="200">

#### Animated Light Settings:
- **Color Curve**: Defines the color animation curve.
  - *Default*: "VertexLighting_ExampleColorCurve" | *Type*: Curve Linear Color
- **Start Position**: Starting point of the color curve.
  - *Default*: 0.0s | *Type*: Float
- **Start at Random Position**: Starts the curve at a random timestamp.
  - *Default*: False | *Type*: Boolean
- **Is Looping**: Determines if the animation repeats. If False, the light is destroyed after playback.
  - *Default*: True | *Type*: Boolean
- **Play Rate**: Speed of the animation playback.
  - *Default*: 100% | *Type*: Float
- **Should Update No Tick Rate**: Forces updates for animated lights even when tick rate updates are disabled.
  - *Default*: False | *Type*: Boolean

---

## Troubleshooting

### **I can't see any vertex lights**
Ensure the following:
- A single `VertexLighting_Manager` is present in the level.
- You have placed `VertexLighting_Light` or `VertexLighting_Light_Animated` actors in your level instead of regular lights.
- The `VertexLights_MaterialFunction` is properly multiplied with your material's texture.
- The model has sufficient vertices within the light's influence range. Vertex lights only illuminate vertices, so ensure your model has enough for visible effects.
