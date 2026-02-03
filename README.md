# Lung Nodules detection
## Dataset
## 3D Reconstruction

For C++ visualization, the following libraries are required: GLAD, GLFW, and GLM.

To run:

```bash
pip install -r requirements.txt
python read.py
cd 3d
./run.sh
```

Hold left click and move the mouse to rotate the camera. Use the left/right arrow keys to adjust the horizontal slice and top/down to adjust vertical cut.
## Detection
Use prepare.ipynb to prepare data

Example:

<img width="354" height="333" alt="image" src="https://github.com/user-attachments/assets/a95fc8c9-2f6a-4486-b476-10b9f0365abe" />

Then use train.py to train model which uses unet and restnet34 backbone.

<img width="692" height="23" alt="image" src="https://github.com/user-attachments/assets/c36c9e90-71bd-41b2-bed3-e3d8d00b8b8a" />

Due to hardware limitations, the model does not achieve its full potential accuracy. The program relies on 2D images instead of a 3D representation, and the neural network architecture is not sufficiently complex for this task.
