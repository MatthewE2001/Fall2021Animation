import maya.cmds as cmds
from math import pow, sqrt

debug = True


def main():

    # Get the current selection, limited to locators (transforms) and joints
    selection = cmds.ls(sl=True, type=('transform', 'joint'))

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

    # Get the current selection, and treat it as root
    root = selection[0]

    # Get all the descendants of the root
    segments = cmds.listRelatives(root, ad=True, ni=True)
    # If the root is a transform and the list of descendants is less than or equal to one in length, it has no children
    if cmds.objectType(root) == 'transform' and len(segments) <= 1:
        cmds.error("Root has no children. A single joint or locator with children"
                   " must be selected as a root for export.", n=True)
        return
    # If the root is a joint and has no descendants, it has no children
    elif segments is None:
        cmds.error("Root has no children. A single joint or locator with children"
                   " must be selected as a root for export.", n=True)
        return

    # If the root is a transform, remove the shape node automatically
    if cmds.objectType(root) == 'transform':
        segments.pop(0)
    else:
        None
    # Add the root into the list as the first segment
    # segments.insert(0, root)

    # Create the HTR header and write the basic data
    htrFile.write("[Header]\n")
    htrFile.write("FileType\thtr\n")
    htrFile.write("DataType\tHTRS\n")
    htrFile.write("FileVersion\t1\n")

    # Prepare and write the segment data to the HTR header
    numSegments = len(segments)
    htrFile.write("NumSegments\t" + str(numSegments) + "\n")

    # Prepare and write the frame data to the HTR header
    numFrames = int(cmds.playbackOptions(maxTime=True, query=True) - cmds.playbackOptions(minTime=True, query=True))
    htrFile.write("NumFrames\t" + str(numFrames) + "\n")

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
    htrFile.write("DataFrameRate\t" + str(dataFrameRate) + "\n")

    # Write the euler rotation order to the HTR header
    htrFile.write("EulerRotationOrder\tZYX\n")

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
    htrFile.write("CalibrationUnits\t" + calibrationUnits + "\n")

    rawRotationUnits = cmds.currentUnit(query=True, angle=True)
    if rawRotationUnits == "rad" or rawRotationUnits == "radian":
        rotationUnits = "Radians"
    else:
        # Default to degrees
        rotationUnits = "Degrees"
    # Write the rotation unit data to the HTR header
    htrFile.write("RotationUnits\t" + rotationUnits + "\n")

    # Write the gravity axis data to the HTR header
    # Y is the constant axis
    htrFile.write("GlobalAxisOfGravity\tY\n")

    # Write the bone length axis data to the HTR header
    # Y is the constant axis
    htrFile.write("BoneLengthAxis\tY\n")

    # Write the scale factor data to the HTR header
    # 1.00 is the constant axis
    htrFile.write("ScaleFactor\t1.00\n")

    # Write the segment hierarchy data
    htrFile.write("[SegmentNames&Hierarchy]\n")
    for segment in segments:
        # If the parent is the root, the parent is written as GLOBAL
        if str(cmds.listRelatives(segment, p=True)[0]) == root:
            htrFile.write(str(segment) + "\t" + "GLOBAL" + "\n")
        # Write the child and parent to the file
        else:
            htrFile.write(str(segment) + "\t" + str(cmds.listRelatives(segment, p=True)[0]) + "\n")

    htrFile.write("[BasePosition]\n")
    for segment in segments:
        # Write Segment Name Tx, Ty, Tz, Rx, Ry, Rz, BoneLength
        # Set the current time to frame 0 (base pose)
        cmds.currentTime(0)

        # Get the values for writing (relative to the parent)
        segmentName = str(segment)
        tx, ty, tz = cmds.xform(segment, query=True, t=True)
        rx, ry, rz = cmds.xform(segment, query=True, ro=True)

        parent_tx, parent_ty, parent_tz = cmds.xform(cmds.listRelatives(segment, p=True), query=True, t=True)

        # Get the bone length using the distance formula between the child and parent
        boneLength = sqrt(pow(tx - parent_tx, 2) + pow(ty - parent_ty, 2) + pow(tz - parent_tz, 2))

        htrFile.write(segmentName + "\t" +
                      "{:.6f}".format(tx) + "\t" +
                      "{:.6f}".format(ty) + "\t" +
                      "{:.6f}".format(tz) + "\t" +
                      "{:.6f}".format(rx) + "\t" +
                      "{:.6f}".format(ry) + "\t" +
                      "{:.6f}".format(rz) + "\t" +
                      "{:.6f}".format(boneLength) + "\n")

    for segment in segments:
        htrFile.write("[" + str(segment) + "]\n")

    htrFile.write("[EndOfFile]\n")

    # Close the HTR file
    htrFile.close()

    # If in debug mode, print a copy of the written file contents to the console
    if debug:
        print ("Written Data:\n" + file(htrFilePath).read())

    return


main()