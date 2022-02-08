
#ifndef CPLAYER
#define CPLAYER

#include "CMesh.hpp"
#include "CGround.hpp"
#include "CColider.hpp"

class CPlayer : public CMesh
{
public:
    CGround *ground;
    glm::vec3 direction;
    float angle;

    CAABBCollider playerCollider;
    std::vector<CAABBCollider> collidingObjects;

    CPlayer() {}
    CPlayer(std::string objfile, CGround *cground, glm::vec3 initPos = glm::vec3(0, 0, 0)) : CMesh(objfile)
    {
        this->ground = cground;
        float yAxis = this->ground->getAltitute(initPos);
        this->position = glm::vec3(initPos.x, yAxis, initPos.z);
        this->direction = glm::vec3(0.0, 0.0, 1.0);
        this->angle = 0.0f;
    }

    void addPlayerColider(CAABBCollider _pCollider)
    {
        this->playerCollider = _pCollider;
    }

    void addColider(CAABBCollider colliderObj)
    {
        collidingObjects.push_back(colliderObj);
    }

    void Move(float value)
    {
        glm::vec3 oldPosition = this->position;

        //  new position
        this->position = this->position + this->direction * value;
        this->position.y = this->ground->getAltitute(this->position);

        // check for standard collision
        bool isColliding = false;
        for (CAABBCollider collObj : this->collidingObjects)
        {
            isColliding = isColliding || this->playerCollider.isCollision(&collObj);
        }
        if (abs(this->position.y - oldPosition.y) > 0.05 || isColliding)
        {
            this->position = oldPosition;
        }
    }

    void setPosition(glm::vec3 xyzCoords)
    {
        this->position = xyzCoords;
    }

    void rotate(float angle)
    {
        this->rotation.y += angle;

        this->direction.z = cos(this->rotation.y);
        this->direction.x = sin(this->rotation.y);
    }
};

class PackageRobot : public CPlayer
{
public:
    int packageCount;
    int maxPackageCount;
    int terminalPackageCount = 0;
    std::vector<int> collectedPackages;

    CAABBCollider terminalCollider;
    std::vector<CAABBCollider> packageColliderObjects;

    PackageRobot() {}

    PackageRobot(std::string objfile, CGround *cground, glm::vec3 initPos = glm::vec3(0, 0, 0)) : CPlayer(objfile, cground, initPos)
    {
        this->packageCount = 0;
        this->maxPackageCount = 3;
        this->terminalPackageCount = 0;
    }

    void setPackageTerminalCollider(CAABBCollider _pCollider)
    {
        this->terminalCollider = _pCollider;
    }

    void addPackageCollider(CAABBCollider packageCollider)
    {
        packageColliderObjects.push_back(packageCollider);
    }

    void setPosition(glm::vec3 xyzCoords)
    {
        this->position = xyzCoords;
        this->position.y = this->ground->getAltitute(this->position);
    }

    void Drive(float value)
    {
        this->Move(value);
        // check for package collission
        int index = 0;
        for (CAABBCollider packageCol : this->packageColliderObjects)
        {
            // check if package is already collected
            bool isCollected = false;
            for (int pkgInx : this->collectedPackages)
            {
                isCollected = isCollected || index == pkgInx;
            }
            if (!isCollected)
            {
                bool isColliding = this->playerCollider.isCollision(&packageCol);
                if (this->packageCount < maxPackageCount && isColliding)
                {
                    this->packageCount += 1;
                    this->collectedPackages.push_back(index);
                }
            }
            index++;
        }
        //  check for terminal collission
        bool isTerminalColision = this->playerCollider.isCollision(&this->terminalCollider);
        if (isTerminalColision && this->packageCount > 0)
        {
            this->terminalPackageCount += this->packageCount;
            this->packageCount = 0;
        }
    }

    void Reset () {
        this->packageCount = 0;
        this->terminalPackageCount = 0;
        this->collectedPackages.clear();
        this->packageColliderObjects.clear();
    }
};

#endif
