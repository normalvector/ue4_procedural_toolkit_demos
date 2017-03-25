# UE4 Procedural Toolkit
This is a library for generating procedural content inside UE4.

At present this should be considered **alpha quality** as code needs more bullet-proofing, documentation, and any and all APIs may change.

<a name="MeshDeformationComponent"></a>
## MeshDeformationComponent
Full mesh deformation toolkit inside UE4, including tools (Move, Scale, Rotate, Spherize etc) based on popular 3d modelling packages, and a set of utilities to control which parts of a mesh will be affected.

In order to allow complex effects this supports:
* Multiple transformations, either using the same selection set or different ones.
* Combining selections for greater control.
* Can be extended in Blueprint to add custom transformations and selection controls.

Variables stored inside MeshDeformationComponent:
| Variable | Description |
|---|---|
| MeshGeometry | The current geometry of the mesh being processed |

### Nodes
There are four basic types of node in the MeshDeformationComponent, these are:
* **Load Geometry** which loads geometry into the MDC ready for processing.
* **Output Geometry** which takes the geometry stored in the MDC and outputs it somewhere else.
* **Select Geometry** which returns [SelectionSets](#SelectionSet) which allow control of where a transformation will affect.
* **Transform Geometry** which modify the geometry loaded into the MDC in different ways.  All *Transform Geometry* nodes will take an optional [SelectionSets](#SelectionSet) which can be used to affect the strength of the transform on a per-vertex basis.

The nodes supported are:

#### **LoadFromStaticMesh** (Load Geometry)
Loads some source geometry from a static mesh.

|Pin| In/Out | Description |
|---|---|---|
| StaticMesh | In | The mesh we're copying the geometry from |
| LOD | In | A StaticMesh can have multiple meshes for different levels of detail, this specifies which LOD we're taking the information from |
| Return | Out |A bool indicating success on reading the geometry |
| MeshDeformationComponent | Out | Returns the target component for easy chaining |

#### **UpdateProceduralMeshComponent** (Output Geometry)
Copies the geometry currently in the MeshDeformationComponent to a ProceduralMeshComponent (as provided by UE4) which can then be used to display it.


|Pin| In/Out | Description |
|---|---|---|
| createCollision | In | Should we ask the ProceduralMeshComponent to create collision? |
| Return | Out |A bool indicating success on writing the geometry |
| MeshDeformationComponent | Out | Returns the target component for easy chaining |

#### **SelectAll** (Select Geometry)
Return a [SelectionSet](#SelectionSet) where all of the vertices are selected at full strength.

|Pin| In/Out | Description |
|---|---|---|
| Return | Out | [SelectionSet](#SelectionSet) with all vertices selected |

#### **SelectNear** (Select Geometry)
Soft linear-falloff radial selection of vertices near the point provided.

|Pin| In/Out | Description |
|---|---|---|
| Center | In | The center of the selection |
| InnerRadius | In | All points inside this distance will be selected with maximum strength |
| OuterRadius | In | All points outside this distance will not be selected |
| Return | Out | [SelectionSet](#SelectionSet) with all vertices selected |

#### **SelectFacing** (Select Geometry)
Soft linear-fallout selection of vertices based on their surface normals.

|Pin| In/Out | Description |
|---|---|---|
| Facing | In | A vector indicating the direction on selection |
| InnerRadiusInDegrees | In | The maximum angle of full effect |
| OuterRadiusInDegrees | In | The maximum angle for any effect.

#### **Jitter** (Transform Geometry)
Modifies geometry by adding a random position to the vertices.  The jitter is controlled by two vectors, *Min* and *Max* with each specifying the limits of the random range for the offsets.

| Pin | In/Out | Description |
|---|---|---|
| RandomStream | In | The stream to use for random numbers |
| Min | In |The per-axis minimums for the jitter's random vector |
| Max | In |The per-axis maximums for the jitter's random vector |
| Selection | In |An optional [SelectionSet](#SelectionSet) controlling the strength of the transformation on a per-vertex basis.  If not provided all vertices will be fully affected. |
| MeshDeformationComponent | Out | Returns the target component to allow easy chaining. |

#### **Translate** (Transform Geometry)
Modifies geometry by adding a constant vector to each vertex's position.

| Pin | In/Out | Description |
|---|---|---|
| Delta | In | The vector to add to each vertex |
| Selection | In |An optional [SelectionSet](#SelectionSet) controlling the strength of the transformation on a per-vertex basis.  If not provided all vertices will be fully affected. |
| MeshDeformationComponent | Out | Returns the target component to allow easy chaining. |

#### **Rotate** (Transform Geometry)
Modifies geometry by rotating each point by a Rotator round a provided center of rotation.

| Pin | In/Out | Description |
|---|---|---|
| Rotation | In | The rotation to be applied to each vertex |
| CenterOfRotation | In | The pivot point for the rotation |
| Selection | In |An optional [SelectionSet](#SelectionSet) controlling the strength of the transformation on a per-vertex basis.  If not provided all vertices will be fully affected. |
| MeshDeformationComponent | Out | Returns the target component to allow easy chaining. |

**TODO**: Add the ability to Lerp the *rotation* of the transform rather than the position of the final points.  Will likely cause much less distortion and better results.

#### **Scale** (Transform Geometry)
Modifies geometry by scaling each point relative to a provided center.

| Pin | In/Out | Description |
|---|---|---|
| Scale3D | In | The per-axis scale factor |
| CenterOfScale | In | The central point for the scaling |
| CenterOfRotation | In | The pivot point for the rotation |
| Selection | In |An optional [SelectionSet](#SelectionSet) controlling the strength of the transformation on a per-vertex basis.  If not provided all vertices will be fully affected. |
| MeshDeformationComponent | Out | Returns the target component to allow easy chaining. |

#### **Transform** (Transform Geometry)
Modifies geometry by applying a full transformation matrix to each vertex.

The transform is applied in UE4's conventional order: Scale -> Rotate -> Translate.

| Pin | In/Out | Description |
|---|---|---|
| Scale3D | In | The per-axis scale factor |
| CenterOfScale | In | The central point for the scaling |
| CenterOfRotation | In | The pivot point for the rotation |
| Selection | In |An optional [SelectionSet](#SelectionSet) controlling the strength of the transformation on a per-vertex basis.  If not provided all vertices will be fully affected. |
| MeshDeformationComponent | Out | Returns the target component to allow easy chaining. |

**TODO:** Same change as Rotate to make things smoother.  May require breaking down the transform into individual operations, or applying a Lerp to the overall transform per-vertex.

#### **Spherize** (Transform Geometry)
Modifies geometry by morphing it towards a sphere of supplied radius and center.

| Pin | In/Out | Description |
|---|---|---|
| SphereRadius | In | The radius of the target sphere |
| SphereCenter | In | The center of the target sphere |
| FilterStrength | In | An overall strength setting, applied on top of Selection |
| Selection | In |An optional [SelectionSet](#SelectionSet) controlling the strength of the transformation on a per-vertex basis.  If not provided all vertices will be fully affected. |
| MeshDeformationComponent | Out | Returns the target component to allow easy chaining. |

**TODO:** Params in C++ in slightly odd order, redo as doc here to put sphere params next to one another.

#### **Inflate** (Transform geometry)
Modifies geometry by moving each vertex along their own normal by the offset provided.

| Pin | In/Out | Description |
|---|---|---|
| Offset | In | The distance to move each vertex along their normal.  +ve="Move outwards", -ve="Move inwards" |
| Selection | In |An optional [SelectionSet](#SelectionSet) controlling the strength of the transformation on a per-vertex basis.  If not provided all vertices will be fully affected. |
| MeshDeformationComponent | Out | Returns the target component to allow easy chaining. |

### TODO (MeshDeformationComponent):
* Allow other sources of geometry
  * Loading .fbx/.obj files at runtime
  * Copying from other MDCs.
  * Primitive geometry factory to create cubes, tori, spheres etc.
* Allow other methods of selecting vertices from the geometry
  * Select near line segment
  * Select near spline curve
  * Select near AABB
  * Select near volume
  * Select by segment
  * Select by VertexColor (Needs a StaticMeshComponent as input, but that can be deleted/hidden in final level)
  * Select by connectivity (May wait for OpenMesh integration)
  * Select by noise function (Needs suitable noise library)
* More outputs
  * Output to .fbx/.obj
  * Can we hook into the StaticMesh system to allow direct output to the Content Browser?
* Better handling for normals- they seem to be autorebuilt at the moment.
* Handling for VertexColors, can we use VC-based Materials on a PMC?
* Check SelectionSet size.
* Stop MeshGeometry from even potentially being nullptr by emptying rather than creating newobject.

<a name="SelectionSet"></a>
## SelectionSet
A SelectionSet allows the selection of items from a collection of items in a weighted manner, where 0=Unselected and 1=Full strength selection.

Internally a SelectionSet contains an array of floats, one per item, in the same order that the items themselves were provided.

Currently the following are implemented as SelectionSets:
* Choosing the vertices of a [MeshGeometry] to affect with a Transform Geometry node.

SelectionSets themselves can be manipulated using the BP nodes in **SelectionSetBPLibrary**.  These include:

### **Clamp (SelectionSet)**
Clamps each value in the SelectionSet to the range provided.

| Pin | In/Out | Description |
|---|---|---|
| Value | In | The SelectionSet to clamp |
| Min | In | The minimum value to clamp to |
| Max | In | The maximum value to clamp to |
| Return | Out | The SelectionSet with the result of the clamp |

### **Ease (SelectionSet)**
Applies an easing function to each value in a SelectionSet, helping to smooth out sharp functions.

| Pin | In/Out | Description |
|---|---|---|
| Value | In | The SelectionSet to ease |
| EaseFunction | In | The Easing Function to apply, from [UE4's supplied list](https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Kismet/EEasingFunc__Type/index.html). |
| Steps | In | The number of steps to ease into, only applies when EaseFunction is '**Steps**' |
| BlendExp | In | The blend exponent for the easing function, controls the shape.  Only applies when EaseFunction is '**EaseIn**', '**EaseOut**', or '**EaseInOut**. |
| Return | Out | The SelectionSet with the result of the ease |

### **SelectionSet + SelectionSet**
Adds the weighting of two selections together.

| Pin | In/Out | Description |
|---|---|---|
| A | In | The first SelectionSet to add |
| B | In | The second SelectionSet to add |
| Result | Out | The sum of the two SelectionSets |

### **SelectionSet + SelectionSet**
Subtracts one selection from another.

| Pin | In/Out | Description |
|---|---|---|
| A | In | The SelectionSet to substract from (**A**-B) |
| B | In | The SelectionSet to subtract (A-**B**) |
| Result | Out | The sum of the two SelectionSets |

### **SelectionSet + Float**
Adds a Float to each value in the SelectionSet, globally altering the strength.

Note that this will apply even to values which were originally zero.

| Pin | In/Out | Description |
|---|---|---|
| Value | In | The SelectionSet to add to |
| Float | In | The floating point value to add |
| Return | Out | The result of SelectionSet+Float |

### **SelectionSet - Float**
Subtracts a Float from each value of the SelectionSet, globally altering the strength.

| Pin | In/Out | Description |
|---|---|---|
| Value | In | The SelectionSet to subtract from |
| Float | In | The floating point value to subtract |
| Return | Out | The result of SelectionSet-Float |

### **Float - SelectionSet**
Subtracts a SelectionSet from a float, effectively inverting the SelectionSet and applying a global modifier at the same time.

| Pin | In/Out | Description |
|---|---|---|
| Float | In | The float to subtract the SelectionSet from |
| Value | In | The SelectionSet to subtract |
| Return | Out | The result of SelectionSet-Float |

### **SelectionSet \* SelectionSet**
Multiplies two SelectionSets together, allowing us to use them to mask one another.

| Pin | In/Out | Descrption |
|---|---|---|
| A | In | The first selection set to multiply |
| B | In | The second selection set to multiply |
| Return | Out | The result of the two SelectionSets multiplied together |

### **SelectionSet \* Float**
Multiples all of the values in a SelectionSet by a provided Float.

This can be used to strengthen a SelectionSet overall without changing unselected items.

| Pin | In/Out | Description |
|---|---|---|
| Value | In | The SelectionSet to multiply |
| Float | In | The Float to multiply the SelectionSet by |
| Result | Out | The result of SelectionSet * Float |

### **SelectionSet / SelectionSet**
Divides all of the values in a SelectionSet by those in another SelectionSet.

Not quite sure of the use of this- wrote it since having +, -, * but not / felt weird.

| Pin | In/Out | Descrption |
|---|---|---|
| A | In | The numerator SelectionSet (**A**&divide;B) |
| B | In | The denominator SelectionSet (A&divide;**B**) |
| Return | Out | The result of the division |

### **SelectionSet \* Float**
Divides all of the values in a SelectionSet by a provided Float.

This can be used to weaken a SelectionSet overall without changing unselected items.

| Pin | In/Out | Description |
|---|---|---|
| Value | In | The SelectionSet to divide |
| Float | In | The Float to divide the SelectionSet by |
| Result | Out | The result of SelectionSet &divide; Float |

### **OneMinus (SelectionSet)**
Applies a OneMinus operation to all weights in a SelectionSet, effectively inverting it (Previously unselected items are now fully selected, previously full-strength items are unselected).

| Pin | In/Out | Description |
|---|---|---|
| Value | In | The SelectionSet to negate |
| Return | Out | The negated SelectionSet |

### **Set (SelectionSet, Float)**
Sets all of the values in a SelectionSet to the provided Float.

This can be useful to quickly build selection sets where all items are selected or unselected based on existing SelectionSets.

| Pin | In/Out | Description |
|---|---|---|
| Value  | In | The SelectionSet to filter |
| Float | In | The Float to set all values to |
| Return | Out | The filtered SelectionSet |

### **Randomize (SelectionSet)**
Scrambles a SelectionSet by setting all values to a random number between the limits provided.

| Pin | In/Out | Description |
|---|---|---|
| RandomStream | In | The stream to get random numbers from |
| Min | In | The minimum value of the random numbers |
| Max | In | The maximum value of the random numbers |
| Return | Out | A SelectionSet with random values between the limits |

### **Max (SelectionSet, SelectionSet)**
Returns a SelectionSet where each value is the maximum from the two provided SelectionSets.

| Pin | In/Out | Description |
|---|---|---|
| A | In | The first SelectionSet to take values from |
| B | In | The second SelectionSet to take values from |
| Result | Out | The result of taking the maximum values from A and B |

### **Min (SelectionSet, SelectionSet)**
Returns a SelectionSet where each value is the minimum from the two provided SelectionSets.

| Pin | In/Out | Description |
|---|---|---|
| A | In | The first SelectionSet to take values from |
| B | In | The second SelectionSet to take values from |
| Result | Out | The result of taking the minimum values from A and B |

### **Max (SelectionSet, Float)**
Filters a SelectionSet so each value is at least equal to Float.

| Pin | In/Out | Description |
|---|---|---|
| Value | In | The SelectionSet to filter |
| Float | In | The value to filter by |
| Result | Out | The filtered SelectionSet |

### **Min (SelectionSet, Float)**
Filters a SelectionSet so each value is at most equal to Float.

| Pin | In/Out | Description |
|---|---|---|
| Value | In | The SelectionSet to filter |
| Float | In | The value to filter by |
| Result | Out | The filtered SelectionSet |

### **Lerp (SelectionSet)**
Blends two SelectionSets together with a straight alpha blend, so Alpha=0 returns A, Alpha=1 returns B, Alpha=0.5 returns (A+B)/2.

| Pin | In/Out | Description |
|---|---|---|
| A | In | The first SelectionSet to blend |
| B | In | The second SelectionSet to blend |
| Alpha | In | The blend factor between the two |
| Return | Out | The blended result |

### **Lerp (SelectionSet, SelectionSet)**
Blends two SelectionSets together with a straight alpha blend, so Alpha=0 returns A, Alpha=1 returns B, Alpha=0.5 returns (A+B)/2.

| Pin | In/Out | Description |
|---|---|---|
| A | In | The first SelectionSet to blend |
| B | In | The second SelectionSet to blend |
| Alpha | In | The blend factor between the two |
| Return | Out | The blended result |

### **Lerp (SelectionSet, Float)**
Blends two SelectionSets together with a straight alpha blend, so Alpha=0 won't change the SelectionSet, Alpha=1 returns will set all values to Float, Alpha=0.5 returns a 50/50 blend of the two.

| Pin | In/Out | Description |
|---|---|---|
| Value | In | The SelectionSet to blend |
| Float | In | The Float to lerp the SelectionSet with |
| Alpha | In | The blend factor between the two |
| Return | Out | The blended result |

### TODO (SelectionSet):
* SelectionSetBPLibrary is currently incorrectly named as StaticSetBPLibrary for some stupid reason.
* Auto-levels node
* 'Ripple' node
* Modulus node
* Check the C++ is right for passing large numbers of UClass-based objects about.
* A 'named LRU cache' to store selection sets in.
* Maybe rename the entire class to something easier?  Is "Selection" taken?  Can we use namespaces for UClass?
* Is there a more generic input name than 'Float'?  Check FVector and similar where multiply by float is a thing.
* Rename 'Value' to 'Selection' to more closely match MeshDeformationComponent?
* Use the word 'filter' more in the docs, it's a good term for what it does.

## MeshGeometry

## General TODO
* Some C++ conventions to check
* Enforce UE4-style naming (UpperCamelCase for variables)
* Check the passing of FRandomStream, should it be reference?
* Add Javadoc-docs based on this document- which can then be used by doxygen to create this document's replacement!
* Support for [Runtime Mesh Component](https://forums.unrealengine.com/showthread.php?113432-Runtime-Mesh-Component-Rendering-high-performance-runtime-generated-meshes!).
* Sort nodes and docs in a sensible order
* Check SelectionSet sizes in operations which take multiple sets.
