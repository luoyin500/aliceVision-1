__version__ = "1.0"

from meshroom.core import desc


class MeshFiltering(desc.CommandLineNode):
    commandLine = 'aliceVision_meshFiltering {allParams}'

    inputs = [
        desc.File(
            name='input',
            label='Input',
            description='''Input Mesh (OBJ file format).''',
            value='',
            uid=[0],
            ),
        desc.FloatParam(
            name='removeLargeTrianglesFactor',
            label='Filter Large Triangles Factor',
            description='Remove all large triangles. We consider a triangle as large if one edge is bigger than N times the average edge length. Put zero to disable it.',
            value=60.0,
            range=(1.0, 100.0, 0.1),
            uid=[0],
            ),
        desc.BoolParam(
            name='keepLargestMeshOnly',
            label='Keep Only the Largest Mesh',
            description='Keep only the largest connected triangles group.',
            value=True,
            uid=[0],
            ),
        desc.IntParam(
            name='iterations',
            label='Nb Iterations',
            description='',
            value=5,
            range=(0, 50, 1),
            uid=[0],
            ),
        desc.FloatParam(
            name='lambda',
            label='Lambda',
            description='',
            value=1.0,
            range=(0.0, 10.0, 0.1),
            uid=[0],
            ),
        desc.ChoiceParam(
            name='verboseLevel',
            label='Verbose Level',
            description='''verbosity level (fatal, error, warning, info, debug, trace).''',
            value='info',
            values=['fatal', 'error', 'warning', 'info', 'debug', 'trace'],
            exclusive=True,
            uid=[],
        ),
    ]

    outputs = [
        desc.File(
            name='output',
            label='Output',
            description='''Output mesh (OBJ file format).''',
            value=desc.Node.internalFolder + 'mesh.obj',
            uid=[],
            ),
    ]
