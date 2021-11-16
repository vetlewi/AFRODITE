
"""
This is the fuction responsible for building the proper setup macros.
"""

command_mapping = {
    'FramePresent': "/Detector/frame",
    'TargetPresent': "/Detector/target",
    'ScatteringChamber': "/Detector/targetchamber",
    'std_slot': {
        ''
    }
}


def DetectorSetup(setup):
    """ This fuction converts the setup
    dictionary to the correct commands to build the
    setup files for the AFRODITE simulation.
    Returns: Setup file as a string
    """

    def SetText(command_path, command):
        return command_path + " " + command

    text = ""

    # Starting with the frame, target and scattering chamber

    if setup['FramePresent'] is not None:
        text += f"/Detector/frame {setup['FramePresent']}"
