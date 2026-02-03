
import SimpleITK as sitk
import matplotlib.pyplot as plt
import numpy as np

from skimage.measure import label,regionprops
from skimage.segmentation import clear_border

from skimage.segmentation import clear_border
from skimage import measure
from skimage.measure import label,regionprops, perimeter
from skimage.morphology import ball, disk, dilation, binary_erosion, remove_small_objects, erosion, closing, reconstruction, binary_closing
from skimage.filters import roberts, sobel
from scipy import ndimage as ndi
import scipy.ndimage
import numpy as np

mhd_file = "data/subset1/1.3.6.1.4.1.14519.5.2.1.6279.6001.287966244644280690737019247886.mhd"

image = sitk.ReadImage(mhd_file)

image_array = sitk.GetArrayFromImage(image)

threshhold = -700

image_array_copy = image_array <= threshhold

import numpy as np

def clearRegion(idx, regions, label_image):
    for coordinates in regions[idx].coords:
        label_image[coordinates[0], coordinates[1]] = 0

def mask_outside(binary):
    cleared = clear_border(binary)

    label_image_lungs = label(cleared)
    
    regions_lungs = regionprops(label_image_lungs)

    areas_lung = [(i, r.area) for i, r in enumerate(regions_lungs)]
    areas_lung_sorted = sorted(areas_lung, key=lambda x: x[1])  

    for el in areas_lung_sorted:
        if el[1] < 30 or (el[0] != len(el) - 1 and el[0] != len(el) - 2):
            if el[1] < 1000:
                clearRegion(el[0], regions_lungs, label_image_lungs)

    binary = label_image_lungs > 0

    edges = roberts(binary)
    binary = ndi.binary_fill_holes(edges)

    return binary

def reconstruction(volume):
    for i in range(volume.shape[0]):
        volume[i] = mask_outside(volume[i])
    
    grid = np.zeros((volume.shape[2], volume.shape[0] + 1, volume.shape[1]))

    for i in range(volume.shape[0]):
        for j in range(volume.shape[1]):
            for k in range(volume.shape[2]):
                grid[k][i][j] = volume[i][j][k] 

    verts, faces, normals, values = measure.marching_cubes(grid, level=0.5)

    [max_x, max_y, max_z] = np.max(verts, axis=0)
    [min_x, min_y, min_z] = np.min(verts, axis=0)

    with open("mesh", "w") as file:
        file.write(f"{len(faces) * 6 * 3 * 2}\n")
        file.write(f"{max_x - min_x} {max_y - min_y} {max_z - min_z}\n")
        for face in faces:
            for el in face[::-1]:
                file.write(f"{verts[el][0] - min_x} {verts[el][1] - min_y} {verts[el][2] - min_z} ")
                file.write(f"{normals[el][0]} {normals[el][1]} {normals[el][2]}\n")
            for el in face:
                file.write(f"{verts[el][0] - min_x} {verts[el][1] - min_y} {verts[el][2] - min_z} ")
                file.write(f"{-normals[el][0]} {-normals[el][1]} {-normals[el][2]}\n")

reconstruction(image_array_copy)
