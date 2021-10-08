####################################################
# File:         ExportHTR.py
# Description:  HTR Skeletal Animation Exporter Script for Maya 2020
# Author:       Benjamin Davies
# Date:         10/08/2021
####################################################

import maya.cmds as cmds
import maya.mel as mel
import maya.api.OpenMaya as om
from math import pow, sqrt

debug = True

ordered_segments = []


def childJoints(segment, htrFile):
    children = cmds.listRelatives(segment, c=True, ni=True, type="joint")
    if children is not None:
        for child in children:
            ordered_segments.append(child)
            htrFile.write(str(child) + "\t" + str(segment) + "\r\n")
            childJoints(child, htrFile)
    else:
        return


def main():

    # Get the current selection, limited to locators (transforms), joints, and splines
    selection = cmds.ls(sl=True, type=('transform', 'joint'))
    print selection

    # Check if multiple objects are selected
    if len(selection) > 1:
        cmds.error("Multiple roots selected. A single joint or locator with children"
                   " must be selected as a root for export.", n=True)
        return
    # Check if no objects are selected
    elif len(selection) == 0:
        cmds.error("Invalid root selection. A single joint or locator with children"
                   " must be selected as a root for export.", n=True)
        return

    # Create a custom export dialog
    htrFilter = "HTR Export (*.htr)"
    htrFilePath = cmds.fileDialog2(cap="Export Selection", ds=2, ff=htrFilter, fm=0)

    # Check if a valid output file has been provided
    if htrFilePath is not None:
        htrFilePath = htrFilePath[0]
        if debug:
            print ("Export Path: " + htrFilePath)
    # If the output file is invalid, the export is cancelled
    else:
        if debug:
            print ("Export Cancelled!")
        return

    # Open/create an HTR file at the path specified by the export dialog
    htrFile = open(htrFilePath, 'w')

    # Get the current selection, and treat it as the global root
    root = selection[0]

    # Get the 'main' joint structure for determining what needs to be written
    root_main = cmds.listRelatives(root, c=True, ni=True, type="joint")[0]
    if root_main is None:
        cmds.error("Root has no child joints. A single joint or locator with children"
                   " must be selected as a root for export.", n=True)
        return
    ordered_segments.append(root_main)

    # Get all the joint descendants of the root, and add the main bone
    segments = cmds.listRelatives(root_main, ad=True, ni=True, type="joint")
    segments.insert(0, root_main)

    # If the main joint has no descendants, it is invalid
    if segments is None:
        cmds.error("Root has no child joints. A single joint or locator with children"
                   " must be selected as a root for export.", n=True)
        return

    # Disable the viewport to speed up the script
    mel.eval("paneLayout -e -manage false $gMainPane")

    # Create the HTR header and write the basic data
    htrFile.write("[Header]\r\n")
    htrFile.write("FileType htr\r\n")
    htrFile.write("DataType HTRS\r\n")
    htrFile.write("FileVersion 1\r\n")

    # Prepare and write the segment data to the HTR header
    numSegments = len(segments)
    htrFile.write("NumSegments " + str(numSegments) + "\r\n")

    # Prepare and write the frame data to the HTR header
    numFrames = int((cmds.playbackOptions(maxTime=True, query=True) + 1) - cmds.playbackOptions(minTime=True, query=True))
    htrFile.write("NumFrames " + str(numFrames) + "\r\n")

    # Prepare the framerate data
    rawFrameRate = cmds.currentUnit(query=True, time=True)
    # Resolve built-in aliases for the framerate
    if rawFrameRate == "game":
        dataFrameRate = 15
    elif rawFrameRate == "film" or rawFrameRate == "23.976":
        dataFrameRate = 24
    elif rawFrameRate == "pal":
        dataFrameRate = 25
    elif rawFrameRate == "ntsc" or rawFrameRate == "29.97fps" or rawFrameRate == "29.97df":
        dataFrameRate = 30
    elif rawFrameRate == "show" or rawFrameRate == "47.952fps":
        dataFrameRate = 48
    elif rawFrameRate == "palf":
        dataFrameRate = 50
    elif rawFrameRate == "ntscf" or rawFrameRate == "59.94":
        dataFrameRate = 60
    elif rawFrameRate == "44100fps":
        dataFrameRate = 44100
    elif rawFrameRate == "48000fps":
        dataFrameRate = 48000
    else:
        # Default to 30
        dataFrameRate = 30
    # Write the framerate data to the HTR header
    htrFile.write("DataFrameRate " + str(dataFrameRate) + "\r\n")

    # Write the euler rotation order to the HTR header
    htrFile.write("EulerRotationOrder ZYX\r\n")

    # Prepare and the unit data
    rawCalibrationUnits = cmds.currentUnit(query=True, linear=True)
    # Resolve built-in aliases for the units
    if rawCalibrationUnits == "mm" or rawCalibrationUnits == "millimeter":
        calibrationUnits = "mm"
    elif rawCalibrationUnits == "cm" or rawCalibrationUnits == "centimeter":
        calibrationUnits = "cm"
    elif rawCalibrationUnits == "m" or rawCalibrationUnits == "meter":
        calibrationUnits = "m"
    elif rawCalibrationUnits == "mk" or rawCalibrationUnits == "kilometer":
        calibrationUnits = "km"
    elif rawCalibrationUnits == "in" or rawCalibrationUnits == "inch":
        calibrationUnits = "in"
    elif rawCalibrationUnits == "ft" or rawCalibrationUnits == "foot":
        calibrationUnits = "ft"
    elif rawCalibrationUnits == "yd" or rawCalibrationUnits == "yard":
        calibrationUnits = "yd"
    elif rawCalibrationUnits == "mi" or rawCalibrationUnits == "mile":
        calibrationUnits = "mi"
    else:
        # Default to mm
        calibrationUnits = "mm"
    # Write the transformation unit data to the HTR header
    htrFile.write("CalibrationUnits " + calibrationUnits + "\r\n")

    rawRotationUnits = cmds.currentUnit(query=True, angle=True)
    if rawRotationUnits == "rad" or rawRotationUnits == "radian":
        rotationUnits = "Radians"
    else:
        # Default to degrees
        rotationUnits = "Degrees"
    # Write the rotation unit data to the HTR header
    htrFile.write("RotationUnits " + rotationUnits + "\r\n")

    # Write the gravity axis data to the HTR header
    # Y is the constant axis
    htrFile.write("GlobalAxisofGravity Y\r\n")

    # Write the bone length axis data to the HTR header
    # Y is the constant axis
    htrFile.write("BoneLengthAxis Y\r\n")

    # Write the scale factor data to the HTR header
    # 1.00 is the constant axis
    htrFile.write("ScaleFactor 1.00\r\n")

    # HIERARCHY
    # Write the segment hierarchy data marker
    htrFile.write("[SegmentNames&Hierarchy]\r\n")
    # Write out the root with a GLOBAL parent
    htrFile.write(str(root_main) + "\tGLOBAL\r\n")
    # Recursively iterate through all child joints, building a hierarchy
    childJoints(root_main, htrFile)

    # BASE POSITION
    htrFile.write("[BasePosition]\r\n")
    for segment in ordered_segments:
        # Write Segment Name Tx, Ty, Tz, Rx, Ry, Rz, BoneLength
        # Set the current time to frame 0 (base pose)
        cmds.currentTime(0)

        # Get the values for writing (relative to the parent)
        segmentName = str(segment)
        print(segment)
        tx, ty, tz = cmds.xform(segment, query=True, t=True)
        rx, ry, rz = cmds.xform(segment, query=True, ro=True)
        matrix = cmds.xform(segment, q=True, m=True)
        print matrix

        # Use the transformation to get the proper rotation values
        m = om.MMatrix(matrix)
        mt = om.MTransformationMatrix(m)
        rot = mt.rotation(asQuaternion=False)

        # Get parent values
        parent_tx, parent_ty, parent_tz = cmds.xform(cmds.listRelatives(segment, p=True), query=True, t=True)

        # Get the bone length using the distance formula between the child and parent
        boneLength = sqrt(pow(tx - parent_tx, 2) + pow(ty - parent_ty, 2) + pow(tz - parent_tz, 2))

        htrFile.write(segmentName + "\t" +
                      "{:.6f}".format(tx) + "\t" +
                      "{:.6f}".format(ty) + "\t" +
                      "{:.6f}".format(tz) + "\t" +
                      "{:.6f}".format(om.MAngle(rot.x).asDegrees()) + "\t" +
                      "{:.6f}".format(om.MAngle(rot.y).asDegrees()) + "\t" +
                      "{:.6f}".format(om.MAngle(rot.z).asDegrees()) + "\t" +
                      "{:.6f}".format(boneLength) + "\r\n")

    # Iterate through segments when writing animation data
    for segment in ordered_segments:
        htrFile.write("[" + str(segment) + "]\r\n")
        # Iterate through the keyframes
        for time in range(0, numFrames):
            cmds.currentTime(time)

            # Check if any of the values have changed since the last keyframe, to determine if
            # the keyframe should be written

            tx, ty, tz = cmds.xform(segment, query=True, t=True)
            rx, ry, rz = cmds.xform(segment, query=True, ro=True)
            sf = 1.000000

            htrFile.write(str(int(time)) + "\t" +
                          "{:.6f}".format(tx) + "\t" +
                          "{:.6f}".format(ty) + "\t" +
                          "{:.6f}".format(tz) + "\t" +
                          "{:.6f}".format(rx) + "\t" +
                          "{:.6f}".format(ry) + "\t" +
                          "{:.6f}".format(rz) + "\t" +
                          "{:.6f}".format(sf) + "\r\n")

    # Write the file footer
    htrFile.write("[EndOfFile]\r\n")

    # Close the HTR file
    htrFile.close()

    # Enable the viewport once all the animation data has been written
    mel.eval("paneLayout -e -manage true $gMainPane")

    # If in debug mode, print a copy of the written file contents to the console
    if debug:
        print ("Written Data:\n" + file(htrFilePath).read())

    return


main()