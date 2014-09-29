package cs335;

import com.obj.Vertex;
import com.sun.opengl.util.GLUT;
import java.awt.geom.Line2D;
import java.util.ArrayList;
import javax.media.opengl.GL;
import java.util.Timer;

public class CarPhysics {

    private boolean accelerate = false; // flag for accelerate
    private boolean decelerate = false; // flag for deaccelerate
    // set variables
    private double maxSpeedPlayer = 1.0; // max speed of player controlled car
    private final double accelerationPlayer = 0.2; // accelaration of player controlled car
    private final double decelerationPlayer = -0.2; //deacceleration of player controlled car
    private final double frictionPlayer = 0.1; // friction of player controlled car
    private final double rotateDelta = 10; // 30 degrees rotate for each key hit
    // delta variables
    private double vectorPlayer = 0; // combined vector of vectorZ and vectorX
    private double vectorZ = 0; // velocity vector (z axis = forward/backward)
    private double vectorX = 0; // velocity vector (x axis = right/left)
    private double anglePlayer = 0; // angle of player controlled car
    public double radianPlayer = 0; // anglePlayer in radian
    public double totalZPlayer = 28; // keeps track the displacement of Z axis of player controlled car
    public double totalXPlayer = -5; // keeps track the displacement of X axis of player controlled car
    // collision detection
    Line2D.Double line1;
    Line2D.Double line2;
    Line2D.Double line3;
    double region = 2;
    public Mesh outsideEdge;
    public Mesh insideEdge;
    // timer and 
    Line2D.Double startLine;
    private double startLoop = 0;
    private double loopDst;
    private boolean loop = false;
    private int loopCtr = 1;
    //boost
    private double startBoostTime = 0;
    public boolean boost = false;
    private double boostDst;
    // AI
    private double rotateDeltaCPU = 7;
    //CPU1
    private double vectorCPU1 = 0.5;
    private double vectorXCPU1 = 0;
    private double vectorZCPU1 = 0;
    private double angleCPU1 = 0;
    private double radianCPU1 = 0;
    private double totalZCPU1 = 32;
    private double totalXCPU1 = 0;
    //CPU2
    private double vectorCPU2 = 0.8;
    private double vectorXCPU2 = 0;
    private double vectorZCPU2 = 0;
    private double angleCPU2 = 0;
    private double radianCPU2 = 0;
    private double totalZCPU2 = 28;
    private double totalXCPU2 = 0;
    //CPU3
    private double vectorCPU3 = 1.0;
    private double vectorXCPU3 = 0;
    private double vectorZCPU3 = 0;
    private double angleCPU3 = 0;
    private double radianCPU3 = 0;
    private double totalZCPU3 = 32;
    private double totalXCPU3 = -5;

    public void Accelerate() {
        accelerate = true;
    }

    public void Deaccelerate() {
        decelerate = true;
    }

    public void TurnRight() {
        anglePlayer = anglePlayer - rotateDelta;
        // make the angle between 0 and 360
        if (anglePlayer < 0) {
            anglePlayer += 360;
        } else if (anglePlayer > 360) {
            anglePlayer -= 360;
        }
        radianPlayer = anglePlayer * Math.PI / 180;
    }

    public void TurnLeft() {
        anglePlayer = anglePlayer + rotateDelta;
        // make the angle between 0 and 360
        if (anglePlayer < 0) {
            anglePlayer += 360;
        } else if (anglePlayer > 360) {
            anglePlayer -= 360;
        }
        radianPlayer = anglePlayer * Math.PI / 180;
    }

    public void Released() {
        accelerate = false;
        decelerate = false;
    }

    public void updateVectorPlayer() {
        // if accelerating
        if (accelerate == true && vectorPlayer <= maxSpeedPlayer) {
            vectorPlayer += accelerationPlayer;
        }
        // if deaccelerating
        if (decelerate == true && vectorPlayer >= (-1 * maxSpeedPlayer)) {
            vectorPlayer += decelerationPlayer;
        } // if neither, naturally decreases the speed
        else if (accelerate == false && decelerate == false) {
            // if the speed is positive
            if (vectorPlayer >= 0) {
                vectorPlayer -= frictionPlayer;
            } else {
                vectorPlayer += frictionPlayer;
            }
        }
//        System.out.println(anglePlayer);
        // calculate each component
        vectorZ = (-1) * vectorPlayer * Math.sin(radianPlayer);
        vectorX = vectorPlayer * Math.cos(radianPlayer);

        // position
        totalZPlayer = totalZPlayer + vectorZ;
        totalXPlayer = totalXPlayer + vectorX;
    }

    public void updateVectorCPU1() {
        if (angleCPU1 < 0) {
            angleCPU1 += 360;
        } else if (angleCPU1 > 360) {
            angleCPU1 -= 360;
        }
        if(vectorCPU1 < 0.5) {
            vectorCPU1 += 0.1;
        }
        else {
            vectorCPU1 -= 0.1;
        }
        //top left curve
        if (totalXCPU1 >= 48 && totalZCPU1 > 18) {
            if (angleCPU1 < 40 || angleCPU1 > 230) {
                angleCPU1 += rotateDeltaCPU;
            } else if (angleCPU1 > 60 && angleCPU1 < 230) {
                angleCPU1 -= rotateDeltaCPU;
            }
        }//left straight
        else if (totalZCPU1 <= 18 && totalZCPU1 >= -18 && totalXCPU1 > 0) {
            if (angleCPU1 < 80 || angleCPU1 > 270) {
                angleCPU1 += rotateDeltaCPU;
            } else if(angleCPU1 > 100 && angleCPU1 < 270) {
                angleCPU1 -= rotateDeltaCPU;
            }
        }
        // bottom left curve
        else if (totalXCPU1 >= 48 && totalZCPU1 < -18) {
            if (angleCPU1 < 120 || angleCPU1 > 310) {
                angleCPU1 += rotateDeltaCPU;
            } else if(angleCPU1 > 140 && angleCPU1 < 310) {
                angleCPU1 -= rotateDeltaCPU;
            }
        }//bottom straight
        else if (totalXCPU1 <= 48 && totalXCPU1 >= -18 && totalZCPU1 < 0) {
            if (angleCPU1 < 170 && angleCPU1 > 0) {
                angleCPU1 += rotateDeltaCPU;
            } else if(angleCPU1 > 190 && angleCPU1 < 360) {
                angleCPU1 -= rotateDeltaCPU;
            }
        } 
        // bottom right curve
        else if (totalXCPU1 <= -18 && totalZCPU1 < -18) {
            if (angleCPU1 < 210 && angleCPU1 > 40) {
                angleCPU1 += rotateDeltaCPU;
            } else if(angleCPU1 > 230 || angleCPU1 < 40) {
                angleCPU1 -= rotateDeltaCPU;
            }
        }  //right straight
        else if (totalZCPU1 <= 18 && totalZCPU1 >= -18 && totalXCPU1 < 0) {
            if (angleCPU1 < 260 && angleCPU1 > 90) {
                angleCPU1 += rotateDeltaCPU;
            } else if(angleCPU1 > 280 || angleCPU1 < 90) {
                angleCPU1 -= rotateDeltaCPU;
            }
        } // top right curve
        else if (totalXCPU1 <= -18 && totalZCPU1 > 18) {
            if (angleCPU1 < 300 && angleCPU1 > 130) {
                angleCPU1 += rotateDeltaCPU;
            } else if(angleCPU1 > 320 || angleCPU1 < 130) {
                angleCPU1 -= rotateDeltaCPU;
            }
        } //top straight
        else if (totalXCPU1 <= 48 && totalXCPU1 >= -18 && totalZCPU1 > 0) {
            if (angleCPU1 < 350 && angleCPU1 > 180) {
                angleCPU1 += rotateDeltaCPU;
            } else if(angleCPU1 > 10 && angleCPU1 < 180) {
                angleCPU1 -= rotateDeltaCPU;
            }
        } 

        radianCPU1 = angleCPU1 * Math.PI / 180;
        vectorZCPU1 = (-1) * vectorCPU1 * Math.sin(radianCPU1);
        vectorXCPU1 = vectorCPU1 * Math.cos(radianCPU1);

        totalXCPU1 += vectorXCPU1;
        totalZCPU1 += vectorZCPU1;
    }

    public void updateVectorCPU2() {
        if (angleCPU2 < 0) {
            angleCPU2 += 360;
        } else if (angleCPU2 > 360) {
            angleCPU2 -= 360;
        }
        if (vectorCPU2 < 0.8) {
            vectorCPU2 += 0.1;
        } else {
            vectorCPU2 -= 0.1;
        }
        //top left curve
        if (totalXCPU2 >= 48 && totalZCPU2 > 18) {
            if (angleCPU2 < 40 || angleCPU2 > 230) {
                angleCPU2 += rotateDeltaCPU;
            } else if (angleCPU2 > 60 && angleCPU2 < 230) {
                angleCPU2 -= rotateDeltaCPU;
            }
        }//left straight
        else if (totalZCPU2 <= 18 && totalZCPU2 >= -18 && totalXCPU2 > 0) {
            if (angleCPU2 < 80 || angleCPU2 > 270) {
                angleCPU2 += rotateDeltaCPU;
            } else if(angleCPU2 > 100 && angleCPU2 < 270) {
                angleCPU2 -= rotateDeltaCPU;
            }
        }
        // bottom left curve
        else if (totalXCPU2 >= 48 && totalZCPU2 < -18) {
            if (angleCPU2 < 120 || angleCPU2 > 310) {
                angleCPU2 += rotateDeltaCPU;
            } else if(angleCPU2 > 140 && angleCPU2 < 310) {
                angleCPU2 -= rotateDeltaCPU;
            }
        }//bottom straight
        else if (totalXCPU2 <= 48 && totalXCPU2 >= -18 && totalZCPU2 < 0) {
            if (angleCPU2 < 170 && angleCPU2 > 0) {
                angleCPU2 += rotateDeltaCPU;
            } else if(angleCPU2 > 190 && angleCPU2 < 360) {
                angleCPU2 -= rotateDeltaCPU;
            }
        } 
        // bottom right curve
        else if (totalXCPU2 <= -18 && totalZCPU2 < -18) {
            if (angleCPU2 < 210 && angleCPU2 > 40) {
                angleCPU2 += rotateDeltaCPU;
            } else if(angleCPU2 > 230 || angleCPU2 < 40) {
                angleCPU2 -= rotateDeltaCPU;
            }
        }  //right straight
        else if (totalZCPU2 <= 18 && totalZCPU2 >= -18 && totalXCPU2 < 0) {
            if (angleCPU2 < 260 && angleCPU2 > 90) {
                angleCPU2 += rotateDeltaCPU;
            } else if(angleCPU2 > 280 || angleCPU2 < 90) {
                angleCPU2 -= rotateDeltaCPU;
            }
        } // top right curve
        else if (totalXCPU2 <= -18 && totalZCPU2 > 18) {
            if (angleCPU2 < 300 && angleCPU2 > 130) {
                angleCPU2 += rotateDeltaCPU;
            } else if(angleCPU2 > 320 || angleCPU2 < 130) {
                angleCPU2 -= rotateDeltaCPU;
            }
        } //top straight
        else if (totalXCPU2 <= 48 && totalXCPU2 >= -18 && totalZCPU2 > 0) {
            if (angleCPU2 < 350 && angleCPU2 > 180) {
                angleCPU2 += rotateDeltaCPU;
            } else if(angleCPU2 > 10 && angleCPU2 < 180) {
                angleCPU2 -= rotateDeltaCPU;
            }
        } 

        radianCPU2 = angleCPU2 * Math.PI / 180;
        vectorZCPU2 = (-1) * vectorCPU2 * Math.sin(radianCPU2);
        vectorXCPU2 = vectorCPU2 * Math.cos(radianCPU2);

        totalXCPU2 += vectorXCPU2;
        totalZCPU2 += vectorZCPU2;
    }

    public void updateVectorCPU3() {
        if (angleCPU3 < 0) {
            angleCPU3 += 360;
        } else if (angleCPU3 > 360) {
            angleCPU3 -= 360;
        }
        if(vectorCPU3 < 1.0) {
            vectorCPU3 += 0.1;
        }
        else {
            vectorCPU3 -= 0.1;
        }        
        //top left curve
        if (totalXCPU3 >= 48 && totalZCPU3 > 18) {
            if (angleCPU3 < 40 || angleCPU3 > 230) {
                angleCPU3 += rotateDeltaCPU;
            } else if (angleCPU3 > 60 && angleCPU3 < 230) {
                angleCPU3 -= rotateDeltaCPU;
            }
        }//left straight
        else if (totalZCPU3 <= 18 && totalZCPU3 >= -18 && totalXCPU3 > 0) {
            if (angleCPU3 < 80 || angleCPU3 > 270) {
                angleCPU3 += rotateDeltaCPU;
            } else if(angleCPU3 > 100 && angleCPU3 < 270) {
                angleCPU3 -= rotateDeltaCPU;
            }
        }
        // bottom left curve
        else if (totalXCPU3 >= 48 && totalZCPU3 < -18) {
            if (angleCPU3 < 120 || angleCPU3 > 310) {
                angleCPU3 += rotateDeltaCPU;
            } else if(angleCPU3 > 140 && angleCPU3 < 310) {
                angleCPU3 -= rotateDeltaCPU;
            }
        }//bottom straight
        else if (totalXCPU3 <= 48 && totalXCPU3 >= -18 && totalZCPU3 < 0) {
            if (angleCPU3 < 170 && angleCPU3 > 0) {
                angleCPU3 += rotateDeltaCPU;
            } else if(angleCPU3 > 190 && angleCPU3 < 360) {
                angleCPU3 -= rotateDeltaCPU;
            }
        } 
        // bottom right curve
        else if (totalXCPU3 <= -18 && totalZCPU3 < -18) {
            if (angleCPU3 < 210 && angleCPU3 > 40) {
                angleCPU3 += rotateDeltaCPU;
            } else if(angleCPU3 > 230 || angleCPU3 < 40) {
                angleCPU3 -= rotateDeltaCPU;
            }
        }  //right straight
        else if (totalZCPU3 <= 18 && totalZCPU3 >= -18 && totalXCPU3 < 0) {
            if (angleCPU3 < 260 && angleCPU3 > 90) {
                angleCPU3 += rotateDeltaCPU;
            } else if(angleCPU3 > 280 || angleCPU3 < 90) {
                angleCPU3 -= rotateDeltaCPU;
            }
        } // top right curve
        else if (totalXCPU3 <= -18 && totalZCPU3 > 18) {
            if (angleCPU3 < 300 && angleCPU3 > 130) {
                angleCPU3 += rotateDeltaCPU;
            } else if(angleCPU3 > 320 || angleCPU3 < 130) {
                angleCPU3 -= rotateDeltaCPU;
            }
        } //top straight
        else if (totalXCPU3 <= 48 && totalXCPU3 >= -18 && totalZCPU3 > 0) {
            if (angleCPU3 < 350 && angleCPU3 > 180) {
                angleCPU3 += rotateDeltaCPU;
            } else if(angleCPU3 > 10 && angleCPU3 < 180) {
                angleCPU3 -= rotateDeltaCPU;
            }
        } 

        radianCPU3 = angleCPU3 * Math.PI / 180;
        vectorZCPU3 = (-1) * vectorCPU3 * Math.sin(radianCPU3);
        vectorXCPU3 = vectorCPU3 * Math.cos(radianCPU3);

        totalXCPU3 += vectorXCPU3;
        totalZCPU3 += vectorZCPU3;
    }

    public void collisionWallCheckPlayer() {
        double playerDst1, playerDst2, playerDst3;
        int counter = 0;
        while (counter < outsideEdge.vIndices.size()) {
            //Get vertices
            Vertex v1 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter));
            Vertex v2 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter + 1));
            Vertex v3 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter + 2));

            //Calculate line
            line1 = new Line2D.Double(v1.getX(), v1.getZ(), v2.getX(), v2.getZ());
            line2 = new Line2D.Double(v2.getX(), v2.getZ(), v3.getX(), v3.getZ());
            line3 = new Line2D.Double(v3.getX(), v3.getZ(), v1.getX(), v1.getZ());

            playerDst1 = line1.ptSegDist(totalXPlayer, totalZPlayer);
            playerDst2 = line2.ptSegDist(totalXPlayer, totalZPlayer);
            playerDst3 = line3.ptSegDist(totalXPlayer, totalZPlayer);

            if (playerDst1 <= region || playerDst2 <= region || playerDst3 <= region) {
                //top and bottom straight
                if (totalXPlayer <= 43 && totalXPlayer >= -12) {
                    collisionWallPlayer();
                } //side straight
                else if (totalZPlayer <= 12 && totalZPlayer >= -12) {
                    collisionSideWallPlayer();
                } else {
                    collisionOutCirclePlayer();
                }
                break;
            }
            counter = counter + 3;
        }
        counter = 0;
        while (counter < insideEdge.vIndices.size()) {
            //Get vertices
            Vertex v1 = insideEdge.vertices.get(insideEdge.vIndices.get(counter));
            Vertex v2 = insideEdge.vertices.get(insideEdge.vIndices.get(counter + 1));
            Vertex v3 = insideEdge.vertices.get(insideEdge.vIndices.get(counter + 2));

            //Calculate line
            line1 = new Line2D.Double(v1.getX(), v1.getZ(), v2.getX(), v2.getZ());
            line2 = new Line2D.Double(v2.getX(), v2.getZ(), v3.getX(), v3.getZ());
            line3 = new Line2D.Double(v3.getX(), v3.getZ(), v1.getX(), v1.getZ());

            playerDst1 = line1.ptSegDist(totalXPlayer, totalZPlayer);
            playerDst2 = line2.ptSegDist(totalXPlayer, totalZPlayer);
            playerDst3 = line3.ptSegDist(totalXPlayer, totalZPlayer);

            if (playerDst1 <= region || playerDst2 <= region || playerDst3 <= region) {
//                //top left curve
//                if (totalXPlayer >= 43 && totalZPlayer <= 34 && totalZPlayer > 13) {
//                    collisionInCirclePlayer();
//                }
//                // bottom left curve
//                if (totalXPlayer >= 43 && totalZPlayer < -14 && totalZPlayer > -32) {
//                    collisionInCirclePlayer();
//                }
//                // bottom right curve
//                if (totalXPlayer <= -12 && totalZPlayer < -14 && totalZPlayer > -32) {
//                    collisionInCirclePlayer();
//                }
//                // top right curve
//                if (totalXPlayer <= -12 && totalZPlayer <= 34 && totalZPlayer > 13) {
//                    collisionInCirclePlayer();
//                }
                //top and bottom straight
                if (totalXPlayer <= 43 && totalXPlayer >= -12) {
                    collisionWallPlayer();
                }
                //side straight
                if (totalZPlayer <= 13 && totalZPlayer >= -14) {
                    collisionSideWallPlayer();
                } else {
                    collisionInCirclePlayer();
                }
                break;
            }
            counter = counter + 3;
        }
    }

    public void collisionWallCheckCPU1() {
        double cpuDst1, cpuDst2, cpuDst3;
        int counter = 0;
        while (counter < outsideEdge.vIndices.size()) {
            //Get vertices
            Vertex v1 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter));
            Vertex v2 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter + 1));
            Vertex v3 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter + 2));

            //Calculate line
            line1 = new Line2D.Double(v1.getX(), v1.getZ(), v2.getX(), v2.getZ());
            line2 = new Line2D.Double(v2.getX(), v2.getZ(), v3.getX(), v3.getZ());
            line3 = new Line2D.Double(v3.getX(), v3.getZ(), v1.getX(), v1.getZ());

            cpuDst1 = line1.ptSegDist(totalXCPU1, totalZCPU1);
            cpuDst2 = line2.ptSegDist(totalXCPU1, totalZCPU1);
            cpuDst3 = line3.ptSegDist(totalXCPU1, totalZCPU1);

            if (cpuDst1 <= region || cpuDst2 <= region || cpuDst3 <= region) {
                //top and bottom straight
                if (totalXCPU1 <= 43 && totalXCPU1 >= -12) {
                    collisionWallCPU1();
                } //side straight
                else if (totalZCPU1 <= 12 && totalZCPU1 >= -12) {
                    collisionSideWallCPU1();
                } else {
                    collisionOutCircleCPU1();
                }
                break;
            }
            counter = counter + 3;
        }
        counter = 0;
        while (counter < insideEdge.vIndices.size()) {
            //Get vertices
            Vertex v1 = insideEdge.vertices.get(insideEdge.vIndices.get(counter));
            Vertex v2 = insideEdge.vertices.get(insideEdge.vIndices.get(counter + 1));
            Vertex v3 = insideEdge.vertices.get(insideEdge.vIndices.get(counter + 2));

            //Calculate line
            line1 = new Line2D.Double(v1.getX(), v1.getZ(), v2.getX(), v2.getZ());
            line2 = new Line2D.Double(v2.getX(), v2.getZ(), v3.getX(), v3.getZ());
            line3 = new Line2D.Double(v3.getX(), v3.getZ(), v1.getX(), v1.getZ());

            cpuDst1 = line1.ptSegDist(totalXCPU1, totalZCPU1);
            cpuDst2 = line2.ptSegDist(totalXCPU1, totalZCPU1);
            cpuDst3 = line3.ptSegDist(totalXCPU1, totalZCPU1);

            if (cpuDst1 <= region || cpuDst2 <= region || cpuDst3 <= region) {
                //top and bottom straight
                if (totalXCPU1 <= 43 && totalXCPU1 >= -12) {
                    collisionWallCPU1();
                }
                //side straight
                if (totalZCPU1 <= 13 && totalZCPU1 >= -14) {
                    collisionSideWallCPU1();
                } else {
                    collisionInCircleCPU1();
                }
                break;
            }
            counter = counter + 3;
        }
    }

    public void collisionWallCheckCPU2() {
        double cpuDst1, cpuDst2, cpuDst3;
        int counter = 0;
        while (counter < outsideEdge.vIndices.size()) {
            //Get vertices
            Vertex v1 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter));
            Vertex v2 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter + 1));
            Vertex v3 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter + 2));

            //Calculate line
            line1 = new Line2D.Double(v1.getX(), v1.getZ(), v2.getX(), v2.getZ());
            line2 = new Line2D.Double(v2.getX(), v2.getZ(), v3.getX(), v3.getZ());
            line3 = new Line2D.Double(v3.getX(), v3.getZ(), v1.getX(), v1.getZ());

            cpuDst1 = line1.ptSegDist(totalXCPU2, totalZCPU2);
            cpuDst2 = line2.ptSegDist(totalXCPU2, totalZCPU2);
            cpuDst3 = line3.ptSegDist(totalXCPU2, totalZCPU2);

            if (cpuDst1 <= region || cpuDst2 <= region || cpuDst3 <= region) {
                //top and bottom straight
                if (totalXCPU2 <= 43 && totalXCPU2 >= -12) {
                    collisionWallCPU2();
                } //side straight
                else if (totalZCPU2 <= 12 && totalZCPU2 >= -12) {
                    collisionSideWallCPU2();
                } else {
                    collisionOutCircleCPU2();
                }
                break;
            }
            counter = counter + 3;
        }
        counter = 0;
        while (counter < insideEdge.vIndices.size()) {
            //Get vertices
            Vertex v1 = insideEdge.vertices.get(insideEdge.vIndices.get(counter));
            Vertex v2 = insideEdge.vertices.get(insideEdge.vIndices.get(counter + 1));
            Vertex v3 = insideEdge.vertices.get(insideEdge.vIndices.get(counter + 2));

            //Calculate line
            line1 = new Line2D.Double(v1.getX(), v1.getZ(), v2.getX(), v2.getZ());
            line2 = new Line2D.Double(v2.getX(), v2.getZ(), v3.getX(), v3.getZ());
            line3 = new Line2D.Double(v3.getX(), v3.getZ(), v1.getX(), v1.getZ());

            cpuDst1 = line1.ptSegDist(totalXCPU2, totalZCPU2);
            cpuDst2 = line2.ptSegDist(totalXCPU2, totalZCPU2);
            cpuDst3 = line3.ptSegDist(totalXCPU2, totalZCPU2);

            if (cpuDst1 <= region || cpuDst2 <= region || cpuDst3 <= region) {
                //top and bottom straight
                if (totalXCPU2 <= 43 && totalXCPU2 >= -12) {
                    collisionWallCPU2();
                }
                //side straight
                if (totalZCPU2 <= 13 && totalZCPU2 >= -14) {
                    collisionSideWallCPU2();
                } else {
                    collisionInCircleCPU2();
                }
                break;
            }
            counter = counter + 3;
        }
    }

    public void collisionWallCheckCPU3() {
        double cpuDst1, cpuDst2, cpuDst3;
        int counter = 0;
        while (counter < outsideEdge.vIndices.size()) {
            //Get vertices
            Vertex v1 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter));
            Vertex v2 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter + 1));
            Vertex v3 = outsideEdge.vertices.get(outsideEdge.vIndices.get(counter + 2));

            //Calculate line
            line1 = new Line2D.Double(v1.getX(), v1.getZ(), v2.getX(), v2.getZ());
            line2 = new Line2D.Double(v2.getX(), v2.getZ(), v3.getX(), v3.getZ());
            line3 = new Line2D.Double(v3.getX(), v3.getZ(), v1.getX(), v1.getZ());

            cpuDst1 = line1.ptSegDist(totalXCPU3, totalZCPU3);
            cpuDst2 = line2.ptSegDist(totalXCPU3, totalZCPU3);
            cpuDst3 = line3.ptSegDist(totalXCPU3, totalZCPU3);

            if (cpuDst1 <= region || cpuDst2 <= region || cpuDst3 <= region) {
                //top and bottom straight
                if (totalXCPU3 <= 43 && totalXCPU3 >= -12) {
                    collisionWallCPU3();
                } //side straight
                else if (totalZCPU3 <= 12 && totalZCPU3 >= -12) {
                    collisionSideWallCPU3();
                } else {
                    collisionOutCircleCPU3();
                }
                break;
            }
            counter = counter + 3;
        }
        counter = 0;
        while (counter < insideEdge.vIndices.size()) {
            //Get vertices
            Vertex v1 = insideEdge.vertices.get(insideEdge.vIndices.get(counter));
            Vertex v2 = insideEdge.vertices.get(insideEdge.vIndices.get(counter + 1));
            Vertex v3 = insideEdge.vertices.get(insideEdge.vIndices.get(counter + 2));

            //Calculate line
            line1 = new Line2D.Double(v1.getX(), v1.getZ(), v2.getX(), v2.getZ());
            line2 = new Line2D.Double(v2.getX(), v2.getZ(), v3.getX(), v3.getZ());
            line3 = new Line2D.Double(v3.getX(), v3.getZ(), v1.getX(), v1.getZ());

            cpuDst1 = line1.ptSegDist(totalXCPU3, totalZCPU3);
            cpuDst2 = line2.ptSegDist(totalXCPU3, totalZCPU3);
            cpuDst3 = line3.ptSegDist(totalXCPU3, totalZCPU3);

            if (cpuDst1 <= region || cpuDst2 <= region || cpuDst3 <= region) {
                //top and bottom straight
                if (totalXCPU3 <= 43 && totalXCPU3 >= -12) {
                    collisionWallCPU3();
                }
                //side straight
                if (totalZCPU3 <= 13 && totalZCPU3 >= -14) {
                    collisionSideWallCPU3();
                } else {
                    collisionInCircleCPU3();
                }
                break;
            }
            counter = counter + 3;
        }
    }

    public void collisionWallPlayer() {
        anglePlayer = 360 - anglePlayer;
        radianPlayer = anglePlayer * Math.PI / 180;
    }

    public void collisionSideWallPlayer() {
        anglePlayer = 180 - anglePlayer;
        radianPlayer = anglePlayer * Math.PI / 180;
    }

    public void collisionWallCPU1() {
        angleCPU1 = 360 - angleCPU1;
        radianCPU1 = angleCPU1 * Math.PI / 180;
    }

    public void collisionSideWallCPU1() {
        angleCPU1 = 180 - angleCPU1;
        radianCPU1 = angleCPU1 * Math.PI / 180;
    }

    public void collisionWallCPU2() {
        angleCPU2 = 360 - angleCPU2;
        radianCPU2 = angleCPU2 * Math.PI / 180;
    }

    public void collisionSideWallCPU2() {
        angleCPU2 = 180 - angleCPU2;
        radianCPU2 = angleCPU2 * Math.PI / 180;
    }

    public void collisionWallCPU3() {
        angleCPU3 = 360 - angleCPU3;
        radianCPU3 = angleCPU3 * Math.PI / 180;
    }

    public void collisionSideWallCPU3() {
        angleCPU3 = 180 - angleCPU3;
        radianCPU3 = angleCPU3 * Math.PI / 180;
    }

    public void collisionOutCirclePlayer() {
        anglePlayer = anglePlayer + 90;
        radianPlayer = anglePlayer * Math.PI / 180;
    }

    public void collisionOutCircleCPU1() {
        angleCPU1 = angleCPU1 + 90;
        radianCPU1 = angleCPU1 * Math.PI / 180;
    }

    public void collisionOutCircleCPU2() {
        angleCPU2 = 90 + angleCPU2;
        radianCPU2 = angleCPU2 * Math.PI / 180;
    }

    public void collisionOutCircleCPU3() {
        angleCPU3 = 90 + angleCPU3;
        radianCPU3 = angleCPU3 * Math.PI / 180;
    }

    public void collisionInCirclePlayer() {
        anglePlayer = anglePlayer - 90;
        radianPlayer = anglePlayer * Math.PI / 180;
    }

    public void collisionInCircleCPU1() {
        angleCPU1 = angleCPU1 - 90;
        radianCPU1 = angleCPU1 * Math.PI / 180;
    }

    public void collisionInCircleCPU2() {
        angleCPU2 = angleCPU2 - 90;
        radianCPU2 = angleCPU2 * Math.PI / 180;
    }

    public void collisionInCircleCPU3() {
        angleCPU3 = angleCPU3 - 90;
        radianCPU3 = angleCPU3 * Math.PI / 180;
    }

    public void collisionCarCheck() {
        double tempDst1, tempDst2, tempDst3, tempDst4, tempDst5, tempDst6;
        double temp;

        tempDst1 = Math.sqrt(Math.pow(totalXPlayer - totalXCPU1, 2) + Math.pow(totalZPlayer - totalZCPU1, 2));
        tempDst2 = Math.sqrt(Math.pow(totalXPlayer - totalXCPU2, 2) + Math.pow(totalZPlayer - totalZCPU2, 2));
        tempDst3 = Math.sqrt(Math.pow(totalXPlayer - totalXCPU3, 2) + Math.pow(totalZPlayer - totalZCPU3, 2));
        tempDst4 = Math.sqrt(Math.pow(totalXCPU1 - totalXCPU2, 2) + Math.pow(totalZCPU1 - totalZCPU2, 2));
        tempDst5 = Math.sqrt(Math.pow(totalXCPU1 - totalXCPU3, 2) + Math.pow(totalZCPU1 - totalZCPU3, 2));
        tempDst6 = Math.sqrt(Math.pow(totalXCPU2 - totalXCPU3, 2) + Math.pow(totalZCPU2 - totalZCPU3, 2));
        
        if (tempDst1 < 3) {
            temp = angleCPU1;
            angleCPU1 = anglePlayer;
            anglePlayer = temp;

            radianCPU1 = angleCPU1 * Math.PI / 180;
            radianPlayer = anglePlayer * Math.PI / 180;

            temp = vectorPlayer;
            vectorPlayer = vectorCPU1;
            vectorCPU1 = temp;
        }
        if (tempDst2 < 3) {
            temp = angleCPU2;
            angleCPU2 = anglePlayer;
            anglePlayer = temp;

            radianCPU2 = angleCPU2 * Math.PI / 180;
            radianPlayer = anglePlayer * Math.PI / 180;

            temp = vectorPlayer;
            vectorPlayer = vectorCPU2;
            vectorCPU2 = temp;
        }
        if (tempDst3 < 3) {
            temp = angleCPU3;
            angleCPU3 = anglePlayer;
            anglePlayer = temp;

            radianCPU3 = angleCPU3 * Math.PI / 180;
            radianPlayer = anglePlayer * Math.PI / 180;

            temp = vectorPlayer;
            vectorPlayer = vectorCPU3;
            vectorCPU3 = temp;
        }
        if (tempDst4 < 3) {
            temp = angleCPU2;
            angleCPU2 = angleCPU1;
            angleCPU1 = temp;

            radianCPU1 = angleCPU1 * Math.PI / 180;
            radianCPU2 = angleCPU2 * Math.PI / 180;

            temp = vectorCPU2;
            vectorCPU2 = vectorCPU1;
            vectorCPU1 = temp;
        }
        if (tempDst5 < 3) {
            temp = angleCPU3;
            angleCPU3 = angleCPU1;
            angleCPU1 = temp;

            radianCPU1 = angleCPU1 * Math.PI / 180;
            radianCPU3 = angleCPU3 * Math.PI / 180;

            temp = vectorCPU3;
            vectorCPU3 = vectorCPU1;
            vectorCPU1 = temp;
        }        
        if (tempDst6 < 3) {
            temp = angleCPU2;
            angleCPU3 = angleCPU3;
            angleCPU3 = temp;

            radianCPU3 = angleCPU3 * Math.PI / 180;
            radianCPU2 = angleCPU2 * Math.PI / 180;

            temp = vectorCPU2;
            vectorCPU2 = vectorCPU3;
            vectorCPU3 = temp;
        }        
    }

    public void boostCar() {
        boostDst = Math.sqrt((totalXPlayer + 29) * (totalXPlayer + 29) + totalZPlayer * totalZPlayer);

        if (boostDst <= 1.5 && boost == false) {
            boost = true;
            startBoostTime = System.currentTimeMillis();
            maxSpeedPlayer = 2;
        }
    }

    public void loopCar() {
        startLine = new Line2D.Double(3, 40, 3, 20);
        loopDst = startLine.ptSegDist(totalXPlayer, totalZPlayer);

        if (loopDst <= 0.5 && loop == false) {
            System.out.println("Start!!!");
            startLoop = System.currentTimeMillis();
            loop = true;
        } else if (loopDst <= 0.5 && loop == true) {
            System.out.println("Loop" + loopCtr + " time: " + (System.currentTimeMillis() - startLoop) / 1000 + " seconds");
            startLoop = System.currentTimeMillis();
            loopCtr++;
        }
    }

    public void drawCar(GL gl) {
        gl.glTranslated(totalXPlayer, 0, totalZPlayer); // translation
        gl.glRotated(anglePlayer, 0, 1, 0); // angle of car
        gl.glRotated(90, 0, 1, 0);
    }

    public void drawPanel(GL gl) {
        gl.glTranslated(totalXPlayer, 0.35, totalZPlayer); // translation
        gl.glRotated(anglePlayer, 0, 1, 0);
        gl.glTranslated(1.1, 0, 0);
    }

    public void drawCPUCar1(GL gl) {
        gl.glTranslated(totalXCPU1, 0, totalZCPU1);
        gl.glRotated(angleCPU1, 0, 1, 0); // angle of car
        gl.glRotated(90, 0, 1, 0);
    }

    public void drawCPUCar2(GL gl) {
        gl.glTranslated(totalXCPU2, 0, totalZCPU2);
        gl.glRotated(angleCPU2, 0, 1, 0); // angle of car
        gl.glRotated(90, 0, 1, 0);
    }

    public void drawCPUCar3(GL gl) {
        gl.glTranslated(totalXCPU3, 0, totalZCPU3);
        gl.glRotated(angleCPU3, 0, 1, 0); // angle of car
        gl.glRotated(90, 0, 1, 0);
    }

    public void display(GL gl, boolean overviewMode) {

        gl.glLoadIdentity();

        // collision check
        collisionWallCheckPlayer();
        collisionWallCheckCPU1();
        collisionWallCheckCPU2();
        collisionWallCheckCPU3();
        collisionCarCheck();
        loopCar();
        boostCar();
        if ((System.currentTimeMillis() - startBoostTime) / 1000 >= 10) {
            boost = false;
            maxSpeedPlayer = 1.0;
            startBoostTime = 0;
        }
        // translation
        updateVectorPlayer();
        updateVectorCPU1();
        updateVectorCPU2();
        updateVectorCPU3();
    }
}
