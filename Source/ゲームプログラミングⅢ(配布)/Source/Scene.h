#pragma once

//�V�[��
class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	//������
	virtual void Initialize() = 0;

	//������
	virtual void Finalize() = 0;

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�`�揈��
	virtual void Render() = 0;

	//GUI�`��
	virtual void DrawGUI() = 0;

	//�����������Ă��邩
	bool IsReady() const { return ready; }

	//���������ݒ�
	void SetRedy() { ready = true; }

private: 
	bool ready = false;
};
