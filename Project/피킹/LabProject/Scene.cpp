#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}
CScene::~CScene()
{
}
void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	static int mouse_x = 0;
	static int mouse_y = 0;

	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		cout <<"1"<< mouse_x << ", " << mouse_y << endl;

		//CoordTransform(mouse_x, mouse_y);
		break;
	case WM_LBUTTONUP:
		break;
	}
}
XMFLOAT4 CScene::CoordTransform(int x, int y)
{
	int x1 = x / CLIENT_WIDTH;
	int y1 = y / CLIENT_HEIGHT;
	cout <<"2"<< x1 << ", " << y1 << endl;
	int transcoord_x = x * 2 - 1;

	int transcoord_y = y * 2 - 1;
	cout <<"3: "<< transcoord_x << ", " << transcoord_y << endl;

	return XMFLOAT4(transcoord_x, transcoord_y, 1, 0);
}
void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		case WM_KEYDOWN:
			switch (wParam)
			{
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					CExplosiveObject *pExplosiveObject = (CExplosiveObject *)m_ppObjects[int(wParam - '1')];
					pExplosiveObject->m_bBlowingUp = true;
					break;
				}
				case 'Z':
					if (m_pPlayer->m_iItemCount > 0)
					{
						for (int i = 0; i < m_nObjects; i++)
						{
							int distance = CheckDistance(m_pPlayer->GetPosition(), m_ppObjects[i]->GetPosition());
							if (distance < 30)
							{
								CExplosiveObject *pExplosiveObject = (CExplosiveObject *)m_ppObjects[i];
								pExplosiveObject->m_bBlowingUp = true;
								m_pPlayer->m_iItemCount--;
								cout << m_pPlayer->m_iItemCount << "��" << endl;
							}
						}
					}
					break;
				default:
					break;
				case VK_CONTROL:
					m_pPlayer->MakeBullet();
					//m_pPlayer->SetMovingDirection(m_pPlayer->GetMovingDirection());
					break;
			}
			break;
		default:
			break;
	}
}
void CScene::BuildObjects()
{
	CExplosiveObject::PrepareExplosion();

	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 500.0f;
	CWallMesh *pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);
	pWallCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pWallsObject = new CWallsObject();
	m_pWallsObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_pWallsObject->SetMesh(pWallCubeMesh);

	m_pWallsObject->SetColor(RGB(0, 0, 0));
	m_pWallsObject->m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, fHalfDepth);
	m_pWallsObject->m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, fHalfDepth);

	m_pPlayer->m_bPlay = true;

	m_nObjects = 0;
	m_ppObjects = new CGameObject*[200];
	
	m_ppObjects[m_nObjects] = new CExplosiveObject();
	m_ppObjects[m_nObjects++]->InitObject(m_pPlayer->GetPosition(), m_bIsRedObject);
	
	m_pBossObject = new CGameObject;
	MakeBossEnemy(m_pPlayer->GetPosition());
}
void CScene::MakeEnemy(bool objecttype)
{
	m_ppObjects[m_nObjects] = new CExplosiveObject();
	m_ppObjects[m_nObjects++]->InitObject(m_pPlayer->GetPosition(), objecttype);
	m_bIsRedObject = false;
}
void CScene::MakeBossEnemy(XMFLOAT3 pos)
{
	CCubeMesh *pObjectCubeMesh = new CCubeMesh(40.0f, 20.0f, 20.0f);
	pObjectCubeMesh->SetOOBB(XMFLOAT3(0, 0, 0), XMFLOAT3(20.0f, 10.0f, 10.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pBossObject-> SetPosition(0, 0, 450);
	m_pBossObject-> SetMesh(pObjectCubeMesh);
	m_pBossObject-> SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_pBossObject-> SetRotationSpeed(160.0f);
	m_pBossObject->SetMovingDirection(XMFLOAT3(Vector3::Subtract( m_pBossObject->GetPosition(),pos)));
	m_pBossObject->SetMovingRange(350);
	//cout << "�ѽ�ٻ���1" << endl;

	m_pBossObject->SetColor(RGB(255, 0, 0));
	
}

void CScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();

	for (int i = 0; i < m_nObjects; i++) if (m_ppObjects[i]) delete m_ppObjects[i];
	if (m_ppObjects) delete[] m_ppObjects;

	if (m_pWallsObject) delete m_pWallsObject;
}
void CScene::CheckObjectByObjectCollisions()
{
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->m_pObjectCollided = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = (i + 1); j < m_nObjects; j++)
		{
			if (m_ppObjects[i]->m_xmOOBB.Intersects(m_ppObjects[j]->m_xmOOBB))
			{
				m_ppObjects[i]->m_pObjectCollided = m_ppObjects[j];
				m_ppObjects[j]->m_pObjectCollided = m_ppObjects[i];
			}
		}
	}
	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
			float fMovingSpeed = m_ppObjects[i]->m_fMovingSpeed;
			m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppObjects[i]->m_fMovingSpeed = m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppObjects[i]->m_pObjectCollided = NULL;
		}
	}
}
void CScene::OnMouseProcessing(WPARAM state, int x, int y)
{
	if ((state& MK_RBUTTON) != 0)
	{

	}
}
void CScene::CheckPlayerByObjectCollision()
{

	for (int i = 0; i < m_nObjects; ++i)
	{
		if (m_pPlayer->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB) == INTERSECTING)
		{
			if (m_ppObjects[i]->m_bActive)
			{
				m_pPlayer->SetPosition(0, 0, -400);
				m_nObjects = 0;
				m_pBossObject->m_iBossHp = 50;
				//m_pPlayer->m_bActive = false;

			}
		}
		else 	m_pPlayer->m_isCollide = false;
		
	}

}
void CScene::CheckObjectByWallCollisions()
{
	for (int i = 0; i < m_nObjects; i++)
	{
		ContainmentType containType = m_pWallsObject->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB);
		switch (containType)
		{
			case DISJOINT:
			{
				int nPlaneIndex = -1;
				for (int j = 0; j < 6; j++)
				{
					PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
					if (intersectType == BACK)
					{
						nPlaneIndex = j;
						break;
					}
				}
				if (nPlaneIndex != -1) //���� �ε����� �ݻ纤�ͷ� �̵�
				{
					XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
					XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
					XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
				}
				break;
			}
			case INTERSECTS:
			{
				int nPlaneIndex = -1;
				for (int j = 0; j < 6; j++)
				{
					PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
					if (intersectType == INTERSECTING)
					{
						nPlaneIndex = j;
						break;
					}
				}
				if (nPlaneIndex != -1) //���� �ε����� �ݻ纤�ͷ� �̵�
				{
					XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
					XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
					XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
				}
				break;
			}
			case CONTAINS:
				break;
		}
	}
}
void CScene::CheckObjectByBulletCollision() // ���� �Ѿ� �浹üũ
{
	//int num = 0;
	//for (auto d : m_pPlayer->m_vecBullet)
	//{
	//	for (int i = 0; i < m_nObjects; ++i)
	//	{
	//		if (m_ppObjects[i]->m_xmOOBB.Contains(d->m_xmOOBB) == INTERSECTING)
	//		{
	//			cout <<i<< " ��° ������������" << endl;
	//			CExplosiveObject *pExplosiveObject = (CExplosiveObject *)m_ppObjects[i];
	//			pExplosiveObject->m_bBlowingUp = true;	
	//		}
	//	}
	//}
	for (auto iter = m_pPlayer->m_vecBullet.begin(); iter < m_pPlayer->m_vecBullet.end(); ++iter)
	{ //��� �÷��̾��� �Ѿ��� ������� �浹�� �˻��Ѵ�.
		for (int i = 0; i < m_nObjects; ++i)
		{ // ���� ����ŭ �浹�� �˻��Ѵ�.
			if (m_ppObjects[i]->m_xmOOBB.Contains((*iter)->m_xmOOBB) == INTERSECTING)
			{ //�浹�ϸ�
				if (m_ppObjects[i]->m_bIsRedObject == FALSE) //�Ϲ����̸�
				{
					CExplosiveObject *pExplosiveObject = (CExplosiveObject *)m_ppObjects[i];
					pExplosiveObject->m_bBlowingUp = true; //���� ���Ľ�Ų��
					(*iter)->m_bActive = false; // ���� ���̻� �׸��� �ʰ� ��Ȱ��ȭ ��Ų��.
				}
				else
				{
					CExplosiveObject *pExplosiveObject = (CExplosiveObject *)m_ppObjects[i];
					pExplosiveObject->m_bBlowingUp = true;
					m_pPlayer->m_iItemCount++; //z�������� ������ ������Ų��
					(*iter)->m_bActive = false; // ���� �� �̻� �׸��� �ʴ´�
					cout << m_pPlayer->m_iItemCount << endl;// �Ѿ��� ������ ȭ�鿡 ����
				}
			}

		}
	}
}
//������ �Ѿ� �浹üũ
void CScene::CheckBossObjectByObjectCollision()
{
	for (auto iter = m_pPlayer->m_vecBullet.begin(); iter < m_pPlayer->m_vecBullet.end(); ++iter)
	{

			if (m_pBossObject->m_xmOOBB.Contains((*iter)->m_xmOOBB) == INTERSECTING)
			{
				m_pBossObject->m_iBossHp -= 5;
				//�÷��̾� �Ѿ˰� ������ �ε��� �� ���� ���� ü���� 5�� ��´�. ���� ĳ���� 10�� ����� ���
				// 
				if (m_pBossObject->m_iBossHp < 0) //������ ü���� 0���� ������
				{
					m_pBossObject->m_bIsBossObjectAlive = false;
				}
			}
	}
}
void CScene::CheckPlayerByBossBulletCollision()
{
	for (auto iter = m_pBossObject->m_vecBossBullet.begin(); iter < m_pBossObject->m_vecBossBullet.end(); ++iter)
	{//������ ��� �Ѿ˰�
		if (m_pPlayer->m_xmOOBB.Contains((*iter)->m_xmOOBB) == INTERSECTING)
		{// �÷��̾��� OOBB��ġ�� Ȯ���Ͽ� �����ϸ�
			m_pPlayer->SetPosition(0, 0, -490); //�÷��̾��� ��ġ�� �ٲپ� ������ �ٽ� �����ϰ�
			(*iter)->m_bActive = false; // ������ �Ѿ��� ��Ȱ��ȭ ���״�.
			m_pPlayer->m_bActive = false;
		}
	}
}

void CScene::CheckPlayerByBossCollision()
{
	if (m_pBossObject->m_xmOOBB.Contains(m_pPlayer->m_xmOOBB)) //������ �Ѿ��� �����ϸ�

	{
		m_pPlayer->SetPosition(0, 0, 490);  //�÷��̾��� ��ġ�� �ʱ� ��ġ�� �ٲ۴�.
		m_nObjects = 0; //���� �ʱ�ȭ ��Ų��.
	}
}

int CScene::CheckDistance(XMFLOAT3 pos, XMFLOAT3 checkpos)
{
	int distance = sqrt((checkpos.x - pos.x)*(checkpos.x - pos.x)
		+ (checkpos.y - pos.y)*(checkpos.y - pos.y)
		+ (checkpos.z - pos.z)*(checkpos.z - pos.z));
	return distance;
}
void CScene::Animate(float fElapsedTime)
{
	m_fMakeObjectTime += fElapsedTime;
	m_fMakeRedObjectTime += fElapsedTime;
	m_fMakeObjectTRush += fElapsedTime;
	if (m_fMakeObjectTime > 1.0f)
	{
		MakeEnemy(false);
		m_fMakeObjectTime = 0;
	}
	if (m_fMakeRedObjectTime > 10.0f)
	{
		MakeEnemy(true);
		m_fMakeRedObjectTime = 0;
		cout << "����ٻ���" << endl;
	}
	//for (int i = 0; i < m_nObjects; ++i)
	/*{
		m_ppObjects[i]->m_fMakeObjectRushTime += fElapsedTime;
		if(m_ppObjects[i]->m_fMakeObjectRushTime>2.0f)
			m_ppObjects[i]->SetMovingDirection(XMFLOAT3(Vector3::Subtract(m_pPlayer->GetPosition(), m_ppObjects[i]->GetPosition())));
	}*/
	// �÷��̾ ������ �� ���� oobb�ڽ��� ������� �ʱ⿡ �÷��̾��� ��ġ ������ ��� �޾ƿ´�
	// �÷��̾� OOBB��ġ ������Ʈ
	m_pPlayer->m_xmOOBB.Center = m_pPlayer->GetPosition();
	// �Ѿ� OOBB��ġ ������Ʈ
	for( auto d : m_pPlayer->m_vecBullet)
		d->m_xmOOBB.Center = m_pPlayer->GetPosition();
	//�� ������Ʈ ��ġ ������Ʈ
	// �Ѿ� ��Ÿ� Ȯ��
	// �� OOBB��ġ ������Ʈ
	for (int i = 0; i < m_nObjects; i++) 
	{
		//if (m_ppObjects[i]->m_bActive)
		{
			m_ppObjects[i]->Animate(fElapsedTime);
			m_ppObjects[i]->m_xmOOBB.Center = m_ppObjects[i]->GetPosition();
		}
	};
	// ���� OOBB��ġ ������Ʈ
	//cout << m_pBossObject->GetPosition().z << endl;
	m_pBossObject->m_xmOOBB.Center = m_pBossObject->GetPosition();
	m_pBossObject->Animate(fElapsedTime);
	m_pBossObject->SetMovingDirection(XMFLOAT3(Vector3::Subtract(  m_pPlayer->GetPosition(), m_pBossObject->GetPosition())));
	float dis = CheckDistance(m_pPlayer->GetPosition(), m_pBossObject->GetPosition());
	if (dis < 300) // 100���� �̳��� �����ϸ� �÷��̾�� �����Ѵ�
	{
		m_pBossObject->SetMovingSpeed(20);
		cout << "�ѽ�ٻ���2" << endl;

		
		m_pBossObject->m_vecBossBullet.emplace_back(new CBullet(m_pBossObject->GetPosition(), m_pBossObject->GetRight(), m_pBossObject->GetUp(), m_pBossObject->GetUp(), m_pBossObject->m_xmf3Velocity, m_pBossObject->GetMovingDirection()));
		cout << "�ѽ�ٻ���33" << endl;
		
	}
	else m_pBossObject->m_iBulletCount = 0;
		
	for (auto v : m_pBossObject->m_vecBossBullet)		v->Animate(fElapsedTime);
	
	//cout << m_pBossObject->GetPosition().x << ", " << m_pBossObject->GetPosition().y << ", " << m_pBossObject->GetPosition().z << endl;

	//��
	m_pWallsObject->Animate(fElapsedTime);
	if (m_pWallsObject->m_xmOOBB.Contains(XMLoadFloat3(&m_pPlayer->m_xmf3Position)) == DISJOINT)
	{
		m_pPlayer->m_isCollide = true; //�浹�� Ȯ���ϴ� ����
		m_pPlayer->SetColor(RGB(180, 0, 100));
		m_pPlayer->SetPosition(0,0,-490);
	}
	//�÷��̾�
	if (m_pPlayer->m_isCollide) // �÷��̾ ��, ���� �浹�ϸ� �׶����� �ð��� ���.
		m_pPlayer->m_changeColorTime += fElapsedTime;
	if (m_pPlayer->m_changeColorTime > 1)
	{
		m_pPlayer->SetColor(RGB(0, 0, 255));
		m_pPlayer->m_isCollide = false; // 1�ʰ� ������ �浹�ߴٴ� 
	}
	for (int i = 0; i < m_nObjects; ++i)
	{
		if(m_pPlayer->GetPosition().z > m_ppObjects[i]->GetPosition().z )
			m_pPlayer->SetColor(RGB(255, 0, 255));
	}
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fElapsedTime);
	for (auto v : m_pPlayer->m_vecBullet)		v->Animate(fElapsedTime);
	//cout << m_MousePosX << m_MousePosY << endl;
	CheckObjectByWallCollisions();
	CheckObjectByObjectCollisions();
	CheckPlayerByObjectCollision();
	CheckObjectByBulletCollision();
	CheckBossObjectByObjectCollision();
	CheckPlayerByBossBulletCollision();
	CheckPlayerByBossCollision();
}
void CScene::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	
	m_pWallsObject->Render(hDCFrameBuffer, pCamera);
	if (m_pBossObject->m_bIsBossObjectAlive)
	{
		m_pBossObject->Render(hDCFrameBuffer, pCamera);
		
	for (auto v : m_pBossObject->m_vecBossBullet)
		if (v->m_bActive)
			v->Render(hDCFrameBuffer, pCamera);
	}
	if (!m_pBossObject->m_bIsBossObjectAlive||m_pPlayer->m_bActive)
	{
		
		HFONT hFont = ::CreateFont(120, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, L"Century Gothic");
		HFONT hOldFont = (HFONT)::SelectObject(hDCFrameBuffer, hFont);
		SetTextColor(hDCFrameBuffer,RGB(150, 0, 100));
		::TextOut(hDCFrameBuffer, 0.0f, CLIENT_HEIGHT*0.5 - 50, L"GAMEOVER", 10);
		::SelectObject(hDCFrameBuffer, hOldFont);
		::DeleteObject(hFont);
	}
	for (int i = 0; i < m_nObjects; i++)
	{
		if(m_ppObjects[i]->m_bActive)
			m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
	}
	for (auto v : m_pPlayer->m_vecBullet)
	{
		if(v->m_bActive)
			v->Render(hDCFrameBuffer, pCamera);
	}
}
