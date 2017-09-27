from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from numpy import *

################################################
# Script to plot the surfaces of the detectors #
# facing the target. Target is assumed to be   #
# in the origin of the figure. The position of #
# the detectors in spherical coordinates is as #
# following (beam axis is along the y-axis):   #
# x = r*sin(th)*cos(ph)						   #
# y = r*sin(th)*sin(ph)						   #
# z = r*cos(th)								   #
# <Det>		<r[mm]>	  <th[dg]>	<ph[dg]>	   #
# Clover1	  210		90		   0		   #
# Clover2	  210		45		   0		   #
# Clover3	  210	   135		 180		   #
# Clover4	  210		45		 180		   #
# Clover5	  210		90		 270		   #
# Clover6	  210		90		 225		   #
# Clover7	  210		90		 315		   #
# Clover8	  210		90		 180		   #
# LaBr1		  140		90		 135		   #
# LaBr2		  140		45		  90		   #
# The center of both of the particle		   #
# telescopes are mounted at a distance of	   #
# 60 mm from the target at +/- 45 deg from the #
# y-axis. Each of the telescopes are 48 mm     #
# wide and 48 mm high.						   #
#											   #
# The CLOVER detectors consists of 2x2		   #
# crystals that are all 50 mm wide and 50 mm   #
# high. The crystals are 70 mm thick.		   #
# The LaBr detectors are cylidrical 3.5x8".	   #
#											   #
# The target chamber had a diameter of 230 mm  #
# and was about 6 mm thick where the gamma     #
# detecors were pointing.					   #
#											   #
# The mounting distance from the target is 	   #
# assumed to be about 5 mm.					   #
################################################

# Setting up figure.
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Rotation function for rotating a surface about the y-axis.
def RotateY(x, y, z, theta):
	try:
		xnew = copy(x)*cos(theta) + copy(z)*sin(theta)
		ynew = copy(y)
		znew = -copy(x)*sin(theta) + copy(z)*cos(theta)
		return xnew, ynew, znew
	except:
		xnew = x*cos(theta) + z*sin(theta)
		ynew = y
		znew = -x*sin(theta) + z*cos(theta)
		return xnew, ynew, znew

# Rotation function for rotating a surface about the z-axis.
def RotateZ(x,y,z, phi):
	try:
		xnew = copy(x)*cos(phi)-copy(y)*sin(phi)
		ynew = copy(x)*sin(phi)+copy(y)*cos(phi)
		znew = copy(z)
		return xnew,ynew,znew
	except:
		xnew = x*cos(phi)-y*sin(phi)
		ynew = x*sin(phi)+y*cos(phi)
		znew = z
		return xnew,ynew,znew



############## Drawing CLOVER detectors #################################
x_var = 210. # Distance from the target [mm]

# Setting up the surface of the detector.
y_var = linspace(-50., 50., 101) # [mm] 
z_var = linspace(-50., 50., 101) # [mm]
Y, Z = meshgrid(y_var, z_var)

# Variables defining the surface of the detector.
x = x_var*ones(Y.shape)
y = Y
z = Z

# Rotational angles of the detectors.

th = array([0,-pi/4.,pi/4.,-pi/4.,-pi/4.,0,0,0]) # Angle to rotate about the y-axis.
ph = array([0., 0., pi, pi, 3*pi/2., 5*pi/4., 7*pi/4., pi]) # Angle to rotate about he z-axis.

# Loop over all CLOVER detectors.
for i in range(len(th)):
	xn, yn, zn = RotateY(x,y,z,th[i]) # Surface after rotation about the y-axis.
	xn, yn, zn = RotateZ(xn,yn,zn,ph[i]) # Surface after rotation about the z-ais.

	# Here we calculate the angle between each of the crystals and the beam axis (y-axis)
	x0,y0,z0 = RotateY(210., 25., 25., th[i])
	x0,y0,z0 = RotateZ(x0,y0,z0, ph[i])
	th_pp = arccos(y0/sqrt(x0**2 + y0**2 + z0**2))*180./pi
	
	x0,y0,z0 = RotateY(210., 25., -25., th[i])
	x0,y0,z0 = RotateZ(x0,y0,z0, ph[i])
	th_pn = arccos(y0/sqrt(x0**2 + y0**2 + z0**2))*180./pi
	
	x0,y0,z0 = RotateY(210., -25., 25., th[i])
	x0,y0,z0 = RotateZ(x0,y0,z0, ph[i])
	th_np = arccos(y0/sqrt(x0**2 + y0**2 + z0**2))*180./pi
	
	x0,y0,z0 = RotateY(210., -25., -25., th[i])
	x0,y0,z0 = RotateZ(x0,y0,z0, ph[i])
	th_nn = arccos(y0/sqrt(x0**2 + y0**2 + z0**2))*180./pi

	print "Angles: %s, %s, %s, %s" % (th_pp, th_pn, th_np, th_nn) # Print out the angles.

	ax.plot_surface(xn, yn, zn) # Plot the surface.
############## End of drawing CLOVER detectors ###########################



############## Drawing LaBr3(Ce) detectors ###############################
R = 140. # Mounting distance from the target of the LaBr3(Ce) detectors [mm]

r = linspace(0, 44.45, 101) # Radius of the LaBr3(Ce) detectors [mm]
theta = linspace(0, 2*pi, 101) # Detector face is a circle...
TH, R = meshgrid(theta, r)

# Variables defining the surface of the detector.
x = 140.*ones(TH.shape)
y = R*cos(TH)
z = R*sin(TH)

# First LaBr3(Ce) detector is only rotated about the z-axis.
xn,yn,zn = RotateZ(x,y,z,135.*pi/180.)
ax.plot_surface(xn,yn,zn)

# Second LaBr3(Ce) detector is rotated about both z- and y-axis.
xn,yn,zn = RotateY(x,y,z,-pi/4.)
xn,yn,zn = RotateZ(xn,yn,zn,pi/2.)
ax.plot_surface(xn,yn,zn)
############## End of drawing LaBr3(Ce) detectors #########################

############## Drawing of the Si telescopes ###############################

x_var = 60. # Center distance from target [mm].
y_var = 8.*3.*linspace(-1., 1., 101) # Width of telescope [mm]
z_var = 8.*3.*linspace(-1., 1., 101) # Height of telescope [mm]

Y, Z = meshgrid(y_var, z_var)

x = x_var*ones(Y.shape)
y = Y
z = Z

# First telescope.
xn, yn, zn = RotateZ(x,y,z,3*pi/4.)
ax.plot_surface(xn,yn,zn)

# Second telescope.
xn, yn, zn = RotateZ(x,y,z,pi/4.)
ax.plot_surface(xn,yn,zn)
############## End of drawing of the Si telescopes #########################


# Axis limits
ax.set_xlim(-250, 250)
ax.set_ylim(-250, 250)
ax.set_zlim(-250, 250)

# Axis units.
ax.set_xlabel("[mm]")
ax.set_ylabel("[mm]")
ax.set_zlabel("[mm]")
plt.show()
