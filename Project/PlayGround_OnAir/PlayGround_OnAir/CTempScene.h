#pragma once
class CTempScene
{

public:
	CTempScene();
	~CTempScene();
};

class CTempLoadScene : public CTempScene
{
public:
	CTempLoadScene();
	~CTempLoadScene();
};

class CTempInGameScene : public CTempScene
{
public:
	CTempInGameScene();
	~CTempInGameScene();
};
class CTempGameOverScene : public CTempScene
{
public:
	CTempGameOverScene();
	~CTempGameOverScene();
};