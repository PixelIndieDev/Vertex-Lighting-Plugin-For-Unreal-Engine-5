<img src="images/plugin_icon/VertexLights_Plugin_Icon.avif" alt="The icon of the Vertex Lighting Plugin" width="350">   

---   

## Supported Engine Versions
- Unreal Engine 5.5
- Unreal Engine 5.6

   <p style="color:#0F52BA;"> <b>[🗒️NOTE] <a href="https://github.com/PixelIndieDev/Vertex-Lighting-Plugin-For-Unreal-Engine-5/issues/1" target="_self">Why are no other engine versions supported?</a> </b> </p>   

---

<h2 style="color:#8A2BE2;"> <b>📑Limitations</b> </h2>  
<ul>
   <li style="color:#8A2BE2;"> <b>Vertex Lighting does not work on landscapes.</b> </li>
</ul>

---

## How to Download and Install

1. **Download the Plugin**
   - Get the plugin from the GitHub Releases page or by downloading the source code.

2. **Install the Plugin**
   - Place the plugin folder into your project's `Plugins` directory. (Create a `Plugins` folder if it doesn't already exist.)

3. **Enable the Plugin**
   - The plugin should be enabled by default. However, double-check that it is enabled in the **Plugins** menu within Unreal Engine.
   
   <img src="images/guide_images/plugin_enabled.avif" alt="The Vertex Lighting Plugin being enabled" width="400">
   
---

## How to Use

1. **Set Up the Plugin**
   - In the Content Browser, navigate to the Vertex Lighting Plugin directory.
   - Drag and drop the `VertexLighting_Manager` into your level. <img src="images/guide_images/manager_in_folder.avif" alt="A guide image for the VertexLighting_Manager location in the folder" width="700">

2. **Configure Materials**
   - Locate the `VertexLights_MaterialFunction` in the Logic folder within the Vertex Lighting Plugin directory. <img src="images/guide_images/materialfunction_in_folder.avif" alt="A guide image for the materialfunction location in the folder" width="700">
   - Open the materials you want to use and multiply your base color/texture (Input A) with the `VertexLights_MaterialFunction` (Input B). <img src="images/guide_images/materialfunction.avif" alt="A guide image for the materialfunction in a material" width="600">
   - Connect the result of the multiplication to the `Base Color` and/or `Emissive Color` of your material.

3. **Add Lights**
   - Place a `VertexLighting_Light` or `VertexLighting_Light_Animated` actor into your level to add lights. <img src="images/guide_images/actors_in_folder.avif" alt="A guide image for the VertexLighting_Light and VertexLighting_Light_Animated location in the folder" width="700">

---

## Demo Level

The plugin includes a demo level showcasing multiple vertex lighting configurations. Experiment with the light settings to get a feel for their functionality and effects.
<img src="images/guide_images/demo_map.avif" alt="The icon of the VertexLighting_Manager" width="1024">

---

## Plugin Components

### **VertexLighting_Manager**
The `VertexLighting_Manager` is essential for vertex lighting functionality.   
<p style="color:#8A2BE2;"> <b>[📑IMPORTANT] Ensure only <ins>one</ins> manager is present per level.</b> </p>
 
<img src="images/actor_icons/Light_Manager.avif" alt="The icon of the VertexLighting_Manager" width="200">

#### Default Settings:

##### Day Night Cycle:
- **UseDayNightCycleAmbientColor**:  Enables ambient color changes according to the day-night cycle.
  - *Default*: False | *Type*: Boolean
- **StartingTime**: Defines the initial time of day using a 24-hour format.
  - *Default*: 07:00 | *Type*: Time Structure [Integer:Integer]
- **DayNightCycleColorCurve**: Defines the color transitions for the day-night cycle.
  - *Default*: "VertextLighting_ExampleDayNightCycle_ColorCurve" | *Type*: Curve Linear Color
- **SecondsPerMinuteInGame**: Specifies how many real-world seconds equate to one in-game minute.
  - *Default*: 0.01s | *Type*: Float | *Range*: 0.0-Infinite
- **TickRate DayNightCycle**: Time interval between day-night cycle updates. Set to `0.0` to disable updates.
  - *Default*: 0.25s | *Type*: Float | *Range*: 0.0-Infinite
 
###### Time Events:
- **Call OnMinute**:  Enables the event dispatcher that triggers when the time updates by one minute.
  - *Default*: False | *Type*: Boolean
- **Call OnHour**:  Enables the event dispatcher that triggers when the time updates by one hour.
  - *Default*: False | *Type*: Boolean

##### Ambient:
- **AmbientColor**: Default environmental color.
  - *Default*: Hex sRGB - 7C7C7CFF | *Type*: Linear Color
- **SunNormalInfluence**: Controls how directional the vertex light sunlight should be, on a percentage basis. 0% makes the sunlight completely non-directional, while 100% makes it fully directional.
  - *Default*: 0% | *Type*: Float | *Range*: 0.0-100.0

##### Skybox:
- **ChangeSkyboxColorWithDayNightCycle**: Adjusts the linked skybox actor’s overall color based on the `DayNightCycleColorCurve` and time. The linked skybox actor must inherit the `VertexLighting_Skybox_Interface` for this function to work.
  - *Default*: True | *Type*: Boolean
- **Skybox Actor**: Specifies the linked skybox actor.
  - *Default*: None | *Type*: Actor

##### Tickrate:
- **TickrateLightUpdate**: Time interval between light updates. Set to `0.0` to disable updates.
  - *Default*: 0.1s | *Type*: Float | *Range*: 0.0-Infinite
 
##### Optimalization:
- **MaximalLightDistanceOverride**: Overrides the maximum render distance for all the light. Set to `0.0` to disable override.
  - *Default*: 0.0cm | *Type*: Float | *Range*: 0.0-10000.0
- **UnloadOutsideOfView**: This will make any lights unload when hopefully not on screen.
  - *Default*: False | *Type*: Boolean
- **DisableTickingOffScreen**: This will make any lights stop ticking when not on screen. This may cause unintended light functioning.
  - *Default*: False | *Type*: Boolean

##### Debug:
- **ShowCombinedVertexLightInfo**: Enables debug information about the active vertex lights.
  - *Default*: True | *Type*: Boolean
- **ShowVertexLightsRenderedInfo**: Enables debug information about the amount of rendered vertex lights.
  - *Default*: True | *Type*: Boolean
- **ShowCombinedVertexLightInfo**: Enables debug information about the exact day-night cycle time.
  - *Default*: True | *Type*: Boolean

<p style="color:#D2042D;"> <b>[‼️CAUTION] This actor also contains variables in the Technical category. DO NOT CHANGE THESE VARIABLES UNLESS YOU KNOW WHAT YOU'RE DOING!</b> </p>

#### Sun Direction:
The sun's direction is controlled by the rotation of the VertexLighting_Manager and is visually represented by a yellow arrow originating from the manager.   

<img src="images/guide_images/sun_direction.avif" alt="Image of the yellow arrow originating from the manager" width="200">

#### In-Editor Events:
- **Draw All Preview Lights**: Displays all vertex lights in the editor.
- **Unregister All Preview Lights**: Stops displaying vertex lights in the editor.

---

### **VertexLighting_Light**
The `VertexLighting_Light` adds a static or dynamic vertex light to your scene. 
<p style="color:#0F52BA;"> <b>[🗒️NOTE] <i>VertexLighting_Light</i> is not considered relevant for Level Bounds.</b> </p>   
   
<img src="images/actor_icons/Light.avif" alt="The icon of the VertexLighting_Light" width="200">

#### Default Settings:
- **LightColor**: Color of the light.
  - *Default*: Hex sRGB - FFFFFFFF | *Type*: Linear Color
<p style="color:#E1C16E;"> <b>[⚠️WARNING] Overridden by color curves for <i>VertexLighting_Light_Animated</i>.</b> </p>
- **Brightness**: Light intensity.
  - *Default*: 10x | *Type*: Float | *Range*: 0.0-100.0
<p style="color:#E1C16E;"> <b>[⚠️WARNING] Overridden by color curves for <i>VertexLighting_Light_Animated</i>.</b> </p>
- **InfluenceRadius**: Radius of the light's influence.
  - *Default*: 300cm | *Type*: Float | *Range*: 0.0-Infinite
- **InfluencetoNormal**: Controls how directional the vertex light should be, on a percentage basis. 0% makes the light completely non-directional, while 100% makes it fully directional.
  - *Default*: 0% | *Type*: Float | *Range*: 0.0-100.0
- **RenderDistance**: Maximum render distance for the light.
  - *Default*: 10,000cm | *Type*: Float | *Range*: 0.0-10,000.0
- **IsMovable Light**: Makes the light's position dynamic during updates.
  - *Default*: False | *Type*: Boolean
- **ShouldTurnOnInRange**: Determines if the light activates when the player is nearby.
  - *Default*: True | *Type*: Boolean

#### Editor Settings:
- **ShowRenderDistance**: Displays the render distance sphere in the editor.
  - *Default*: False | *Type*: Boolean

---

### **VertexLighting_Light_Animated**
The `VertexLighting_Light_Animated` adds a static or dynamic vertex light with a color curve to your scene.
<p style="color:#0F52BA;"> <b>[🗒️NOTE] The <i>VertexLighting_Light_Animated</i> inherits from <i>VertexLighting_Light</i>.</b> </p>   
   
<img src="images/actor_icons/Light_Animated.avif" alt="The icon of the VertexLighting_Light_Animated" width="200">

#### Animated Light:
- **ColorCurve**: Defines the color animation curve.
  - *Default*: "VertexLighting_ExampleColorCurve" | *Type*: Curve Linear Color
- **StartPosition**: Starting point of the color curve.
  - *Default*: 0.0s | *Type*: Float
- **UseLightColorForPreview**: If true, the color used in the preview will use the 'LightColor' color. If false, it will use the starting 'StartPosition' on the 'ColorCurve' color even if it not a visible light color.
  - *Default*: true | *Type*: Boolean
- **StartatRandom Position**: Starts the curve at a random timestamp.
  - *Default*: False | *Type*: Boolean
- **IsLooping**: Determines if the animation repeats. If False, the light is destroyed after playback.
  - *Default*: True | *Type*: Boolean
- **PlayRate**: Speed of the animation playback.
  - *Default*: 100% | *Type*: Float
- **CurveTickRate**: Time interval between Curve Light updates. Set to `0.0` to disable updates.
  - *Default*: 0.01s | *Type*: Float | *Range*: 0.0-Infinite
- **ShouldUpdateNoTickRate**: Forces updates for animated lights even when tick rate updates are disabled.
  - *Default*: False | *Type*: Boolean

#### Animated Light Icon:
- **IconColorTimeValue01**: This value gets used for the 2 colors that get used in the icon coloring.
  - *Default*: 0.5s | *Type*: Float | *Range*: Infinite
- **IconColorTimeValue02**: This value gets used for the 2 colors that get used in the icon coloring.
  - *Default*: 1.5s | *Type*: Float | *Range*: Infinite

---

## Other Components
### **VertexLights_MaterialFunction**

#### Default Settings:
- **WorldPosition (Optional)**: Allows manual override of the world position. Defaults to the Absolute World Position.
  - *Default*: Absolute World Position | *Type*: Vector3
- **VertexBasedNormals (Optional)**: Input for normals interpolated per vertex.
  - *Default*: VertexNormalWS | *Type*: Vector3

---

### **Color Curve**
`VertexLighting_Light_Animated` uses a color curve for its `Light Color` and `Brightness`

#### Color Channels:
- **R Channel**: Defines the red color
- **G Channel**: Defines the green color
- **B Channel**: Defines the blue color
- **A Channel**: Defines the brightness

---

## Modifying maximum registered vertex lights
To adjust the maximum number of vertex lights that can be registered at the same time:
- In-editor, go to **Edit -> Project Settings -> Plugins -> Vertex Lighting Plugin Settings**
- Change the 'Max Lights' value to the number of your liking.

<p style="color:#D2042D;"> <b>[‼️CAUTION] There you can also change the render target that gets resized when you change the 'Max Lights' value. DO NOT CHANGE THIS VARIABLES UNLESS YOU KNOW WHAT YOU'RE DOING!</b> </p>

---

## Troubleshooting

### **I can't see any vertex lights**
Ensure the following:
- A single `VertexLighting_Manager` is present in the level.
- You have placed `VertexLighting_Light` or `VertexLighting_Light_Animated` actors in your level instead of regular lights.
- The `VertexLights_MaterialFunction` is properly multiplied with your material's texture.
- The model has sufficient vertices within the light's influence range. Vertex lights only illuminate vertices, so ensure your model has enough for visible effects.
- You are not trying to light a [landscape](https://dev.epicgames.com/documentation/en-us/unreal-engine/creating-landscapes-in-unreal-engine).

### **Crash: AnimSequenceBase error**
- Make sure you have the `Animation Compression Library` plugin, made by Epic Games, enabled.

---

## Credit
<p style="color:#0F52BA;"> <b>[🗒️NOTE] Portions of this project are based and improved upon on the vertex lighting implementation originally created by <a href="https://x.com/evilreflex" target="_self">EvilReFlex</a>, shared via Discord.</b> </p>
