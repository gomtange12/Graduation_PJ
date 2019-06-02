global rot = eulerToQuat(eulerAngles 90 0 0)
 
function Y_UP obj =
(
   Select obj
   ResetXForm obj
   modPanel.addModToSelection (XForm ()) ui:on
   obj.modifiers[#XForm].gizmo.rotation += rot as quat
   rotate obj (inverse rot as quat)
   for obj in (selection as array) do ( convertToPoly obj)  
)
 
rollout UTTool "Unity Tool" width:162 height:100
(
   groupBox grp1 "Geometry Tools" pos:[7,3] width:148 height:80
   button btn1 "Finalize Geometry" pos:[18,23] width:127 height:34
   on btn1 pressed  do(
     selectedObjects = getCurrentSelection()
     if selectedObjects.count > 0 then for obj in selectedObjects do Y_UP obj
     else Messagebox "No object selected" title: "Unity Tool"
   )
)
 
createDialog UTTool